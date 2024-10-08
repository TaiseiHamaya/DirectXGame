#include "Particle.hlsli"

struct TransformMatrix {
	float4x4 world;
};

struct CameraInfomation {
	float4x4 viewProjection;
};

struct VertexShaderInput {
	float4 position : POSITION0;
};

uint instanceID : SV_InstanceID;

TransformMatrix gTransformMatrix[] : register(t0);
ConstantBuffer<CameraInfomation> gCameraMatrix : register(b1);

static const float4x4 wvp = mul(gTransformMatrix[instanceID].world, gCameraMatrix.viewProjection);

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;

	output.position = mul(input.position, wvp);
	return output;
}
