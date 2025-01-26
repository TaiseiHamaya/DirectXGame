#include "../Deferred.hlsli"

struct TransformMatrix {
	float4x4 world;
	float4x4 worldIT;
};

struct CameraInfomation {
	float4x4 viewProjection;
	float4x4 view;
};

struct VertexShaderInput {
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
};

ConstantBuffer<TransformMatrix> gTransformMatrix : register(b0);
ConstantBuffer<CameraInfomation> gCameraMatrix : register(b1);

static const float4x4 wITv = mul(gTransformMatrix.worldIT, gCameraMatrix.view);

static const float4x4 wvp = mul(gTransformMatrix.world, gCameraMatrix.viewProjection);

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;

	output.position = mul(input.position, wvp);
	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, (float3x3)wITv));
	return output;
}
