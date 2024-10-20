#include "ParticleMesh.hlsli"

struct ParticleData {
	float4x4 world;
	float4 color;
};

struct CameraInfomation {
	float4x4 viewProjection;
};

struct VertexShaderInput {
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
};

StructuredBuffer<ParticleData> gParticleData : register(t0);
ConstantBuffer<CameraInfomation> gCameraMatrix : register(b0);

VertexShaderOutput main(VertexShaderInput input, uint instanceID : SV_InstanceID) {
	VertexShaderOutput output;
	const float4x4 wvp = mul(gParticleData[instanceID].world, gCameraMatrix.viewProjection);
	output.position = mul(input.position, wvp);
	output.texcoord = input.texcoord;
	output.color = gParticleData[instanceID].color;
	return output;
}
