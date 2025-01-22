#include "Object3d.hlsli"

struct TransformMatrix {
	float4x4 world;
};

struct CameraInfomation {
	float4x4 viewProjection;
};

struct VertexShaderInput {
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
};

ConstantBuffer<TransformMatrix> gTransformMatrix : register(b0);
ConstantBuffer<CameraInfomation> gCameraMatrix : register(b1);

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;

	output.world = mul(input.position, gTransformMatrix.world).xyz;
	output.position = mul(float4(output.world, 1.0f), gCameraMatrix.viewProjection);
	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, (float3x3) gTransformMatrix.world));
	return output;
}
