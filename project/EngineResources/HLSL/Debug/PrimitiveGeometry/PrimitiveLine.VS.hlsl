#include "PrimitiveLine.hlsli"

struct TransformMatrix {
	float4x4 world;
};

struct CameraInfomation {
	float4x4 viewProjection;
};

struct VertexShaderInput {
	float4 position : POSITION0;
};

StructuredBuffer<TransformMatrix> gTransformMatrix : register(t0);
ConstantBuffer<CameraInfomation> gCameraMatrix : register(b0);

VertexShaderOutput main(float4 position : POSITION0, uint index : SV_InstanceID) {
	VertexShaderOutput output;
	static const float4x4 wvp = mul(gTransformMatrix[index].world, gCameraMatrix.viewProjection);

	output.position = mul(position, wvp);
	return output;
}
