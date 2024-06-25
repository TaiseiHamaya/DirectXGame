#include "ChromaticAberration.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
float aberrationLevel : register(b0);

float4 main(VertexShaderOutput input) : SV_TARGET {
	float4 sampleColor1 = gTexture.Sample(gSampler, input.texcoord);
	float4 sampleColor2 = gTexture.Sample(gSampler, input.texcoord + float2(aberrationLevel, 0.0f));
	float4 sampleColor3 = gTexture.Sample(gSampler, input.texcoord - float2(aberrationLevel, 0.0f));
	
	float4 outputColor = float4(sampleColor3.r, sampleColor1.g, sampleColor2.b, 1.0f);
	return outputColor;
}