#include "ChromaticAberration.hlsli"

struct VertexShaderInput {
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	output.position = input.position;
	output.texcoord = input.texcoord;
	return output;
}