#include "PostEffectTest.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

float4 main(VertexShaderOutput input) : SV_TARGET {
	float4 sampleColor = gTexture.Sample(gSampler, input.texcoord);
	float y = 0.333f * sampleColor.r + 0.333f * sampleColor.g+ 0.334f * sampleColor.b;
	float4 outputColor = float4(y, y, y, 1.0f);
	return outputColor;
}