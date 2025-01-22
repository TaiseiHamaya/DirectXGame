#include "Deferred.hlsli"
#include "Tools/PackNormalV2.hlsli"
#include "Tools/PackA2.hlsli"

struct Material {
	float3 color;
	uint lightingType;
	float shininess;
	float4x4 uvTransform;
};

struct PixelShaderOutput {
	float4 albedoShading : SV_Target0; // 10bit*3+2bit unorm(xyz : Albedo, w : ShadingType)
	float3 normalShininess : SV_Target1; // 11bit*2+10bit float(xy : ViewNormalPacked, z : Shininess)
};

struct Camera {
	float3 position;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<Camera> gCamera : register(b1);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	// texture color
	float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	// Dither抜き
	
	// 出力
	output.albedoShading.xyz = textureColor.rgb * gMaterial.color.rgb;
	output.albedoShading.w = PackA2bit(gMaterial.lightingType); // packing
	output.normalShininess.xy = PackingNormalV2(input.normal);
	output.normalShininess.z = gMaterial.shininess;
	return output;
}