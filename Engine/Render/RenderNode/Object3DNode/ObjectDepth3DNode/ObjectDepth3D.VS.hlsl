#include "ObjectDepth3D.hlsli"

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

float LinearDepth(float z, float w, float nearZ, float farZ) {
	return (z * nearZ) / (farZ * w - z * (nearZ + farZ));
}

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	output.position = mul(input.position, gTransformMatrix.WVP);
	output.texcoord = input.texcoord;
	float3 worldPosition = mul(input.normal, (float3x3) gTransformMatrix.world);
	output.normal = normalize(worldPosition);
	//output.depth = output.position.z / 1000.0f;
	output.depth = LinearDepth(output.position.z, output.position.w, 0.1f, 1000.0f);
	return output;
}