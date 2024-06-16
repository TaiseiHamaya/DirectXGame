#include "ZBuffer3d.hlsli"

struct TransformMatrix {
	float4x4 WVP;
	float4x4 world;
};
struct VertexShaderInput {
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
};

ConstantBuffer<TransformMatrix> gTransformMatrix : register(b0);

float LinearDepth(float d, float nearZ, float farZ) {
	return (d - nearZ) / (farZ - nearZ);
}

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	output.position = mul(input.position, gTransformMatrix.WVP);
	output.depth = LinearDepth(output.position.z, 0.1f, 1000.0f);
	return output;
}