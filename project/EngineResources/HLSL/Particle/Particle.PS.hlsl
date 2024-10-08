#include "Particle.hlsli"

struct Material {
	float4 color;
};

struct PixelShaderOutput {
	float4 color : SV_Target0;
};

Material gMaterial[] : register(t0);
//Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input, uint instanceID : SV_InstanceID) {
	PixelShaderOutput output;
	//float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	//float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	// ライティングなし
	//output.color = gMaterial.color * textureColor;
	output.color = gMaterial[instanceID].color;
	// 透明の場合は出力せず終了
	// サンプリングによっては0にならない場合があるためバッファを取る
	if (output.color.a <= 0.05f) {
		discard;
	}
	return output;
}