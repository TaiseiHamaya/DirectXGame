#include "Forward.hlsli"

struct Material {
	float3 color;
	int lightingType;
	float shininess;
	float4x4 uvTransform;
};

struct Camera {
	float3 position;
};

struct DirectionalLight {
	float3 color; // 色
	float intensity; // 輝度
	float3 directon; // 向き
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<Camera> gCamera : register(b1);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b2);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

float4 main(VertexShaderOutput input) : SV_Target0 {
	float4 output;
	float3 transformedUV = mul(float3(input.texcoord, 1.0f), (float3x3)gMaterial.uvTransform);
	float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	float3 baseColor = textureColor.rgb * gMaterial.color;
	
	// specular
	float3 toCamera = normalize(gCamera.position - input.world);
	float3 halfVector = normalize(-gDirectionalLight.directon + toCamera);
	float dotNormal = dot(input.normal, halfVector);
	float specularPow = pow(saturate(dotNormal), gMaterial.shininess);
	if (isnan(specularPow)) {
		specularPow = 0;
	}
	else {
		specularPow *= 2.0f;
	}
	float3 specular = gDirectionalLight.color * gDirectionalLight.intensity * specularPow * baseColor;

	
	// ライティングなし
	if (gMaterial.lightingType == 0) {
		output.rgb = baseColor.rgb;
		output.a = 1.0f;
	}
	// Lambert
	else if (gMaterial.lightingType == 1) {
		float NdotL = dot(input.normal, -gDirectionalLight.directon);
		float cos = saturate(NdotL);
		output.rgb = baseColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
		output.rgb += specular;
		output.a = 1.0f;
	}
	// Half Lambert
	else if (gMaterial.lightingType == 2) {
		float NdotL = dot(input.normal, -gDirectionalLight.directon);
		float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
		output.rgb = baseColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
		output.rgb += specular;
		output.a = 1.0f;
	}
	// それ以外は異常値なので、黒を出力
	else {
		output = float4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	// 透明の場合は出力せず終了
	// サンプリングによっては0にならない場合があるためバッファを取る
	if (output.a <= 0.05f) {
		discard;
	}
	
	return output;
}