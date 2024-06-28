#include "RadialBlur.hlsli"

struct BlurInfo {
	float2 center;
	float power;
	uint sampleCount;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<BlurInfo> gBlurInfo : register(b0);

float4 main(VertexShaderOutput input) : SV_TARGET0 {
	float4 outputColor;
	
	float2 blurDirection = (input.texcoord - gBlurInfo.center) * gBlurInfo.power / gBlurInfo.sampleCount;
	
	float4 sampleColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (uint count = 0; count < gBlurInfo.sampleCount; ++count) {
		sampleColor += gTexture.Sample(gSampler, input.texcoord - blurDirection * count);
	}
	sampleColor /= gBlurInfo.sampleCount;
	sampleColor.a = 1.0f;
	outputColor = sampleColor;
	return outputColor;
}