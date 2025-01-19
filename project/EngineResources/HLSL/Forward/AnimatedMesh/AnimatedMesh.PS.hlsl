#include "AnimatedMesh.hlsli"

struct Material {
	float3 color;
	int lightingType;
	float4x4 uvTransform;
};
struct PixelShaderOutput {
	float4 color : SV_Target0;
};
struct DirectionalLight {
	float3 color; // 色
	float intensity; // 輝度
	float3 directon; // 向き
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	// ライティングなし
	if (gMaterial.lightingType == 0) {
		output.color = float4(gMaterial.color, 1.0f) * textureColor;
	}
	// Lambert
	else if (gMaterial.lightingType == 1) {
		float NdotL = dot(normalize(input.normal), -gDirectionalLight.directon);
		float cos = saturate(NdotL);
		output.color.rgb = gMaterial.color * textureColor.rgb * gDirectionalLight.color * cos * gDirectionalLight.intensity;
		output.color.a = textureColor.a;
	}
	// Half Lambert
	else if (gMaterial.lightingType == 2) {
		float NdotL = dot(normalize(input.normal), -gDirectionalLight.directon);
		float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
		output.color.rgb = gMaterial.color * textureColor.rgb * gDirectionalLight.color * cos * gDirectionalLight.intensity;
		output.color.a = textureColor.a;
	}
	// それ以外は異常値なので、白を出力
	else {
		output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	// 透明の場合は出力せず終了
	// サンプリングによっては0にならない場合があるためバッファを取る
	if (output.color.a <= 0.05f) {
		discard;
	}
	return output;
}