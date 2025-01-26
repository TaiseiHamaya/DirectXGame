#include "FullscreenShader.hlsli"
#include "Tools/PackNormalV2.hlsli"
#include "Tools/PackA2.hlsli"
#include "Tools/ToLinearZBuffer.hlsli"

struct SpotLight {
	float3 color; // 色
	float intensity; // 輝度
	float3 position; // 位置
	float decay; // 距離減衰率
	float3 direction; // 向き
	float distance; // ライト距離
	float angle; // ライト範囲
	float falloffStart; // ライト
};

struct Camera {
	float3 position;
	float4x4 viewInv;
};

Texture2D<float4> gAlbedoShading : register(t0);
Texture2D<float3> gNormal : register(t1);
Texture2D<float> gDepth : register(t2);
SamplerState gSampler : register(s0);

ConstantBuffer<SpotLight> gSpotLight : register(b0);
ConstantBuffer<Camera> gCamera : register(b1);

float4 main(VertexShaderOutput input) : SV_TARGET {
	float4 output;
	
	// sampling
	float4 albedoShading = gAlbedoShading.Sample(gSampler, input.texcoord.xy);
	float3 normalViewShininess = gNormal.Sample(gSampler, input.texcoord.xy);
	float ndcDepth = gDepth.Sample(gSampler, input.texcoord.xy);
	
	// unpack
	float3 albedo = albedoShading.rgb;
	uint shadingType = UnpackA2bit(albedoShading.a);
	float3 normal = mul(UnpackingNormaV2(normalViewShininess.xy), (float3x3)gCamera.viewInv);
	float shininess = normalViewShininess.z;
	//float3 normal = normalize(normalShininess.xyz);
	float3 ndc = float3(input.texcoord.xy * 2.0f - 1, ToLinearZBuffer(ndcDepth, 0.1f, 1000.0f));
	float3 world = mul(float4(ndc, 1.0f), gCamera.viewInv).xyz;
	
	// specular
	float3 toCamera = normalize(gCamera.position - world);
	float3 direction = normalize(world - gSpotLight.position);
	// 距離減衰
	float distance = length(world - gSpotLight.position);
	float distanceDecayFactor = pow(saturate(-distance / gSpotLight.distance + 1.0f), gSpotLight.decay);
	// 範囲減衰
	float cosAngle = dot(direction, gSpotLight.direction);
	float falloffFactor = saturate((cosAngle - gSpotLight.angle) / (gSpotLight.falloffStart - gSpotLight.angle));
	// ライト色
	float3 lightColor = gSpotLight.color.rgb * gSpotLight.intensity * distanceDecayFactor * falloffFactor;

	
	float3 halfVector = normalize(-direction + toCamera);
	float dotNormal = dot(normal, halfVector);
	float specularPow = pow(saturate(dotNormal), shininess);
	if (isnan(specularPow)) {
		specularPow = 0;
	}
	else {
		specularPow *= 2.0f;
	}
	float3 specular = gSpotLight.color * gSpotLight.intensity * specularPow * albedo;

	
	// ライティングなし
	if (shadingType == 0) {
		output.xyz = albedo;
		output.a = 1.0f;
	}
	// Lambert
	else if (shadingType == 1) {
		float NdotL = dot(normal, -direction);
		float cos = saturate(NdotL);
		float3 baseColor = albedo * gSpotLight.color;
		output.rgb = baseColor * cos * gSpotLight.intensity;
		output.rgb += specular;
		output.a = 1.0f;
	}
	// Half Lambert
	else if (shadingType == 2) {
		float NdotL = dot(normal, -direction);
		float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
		float3 baseColor = albedo * gSpotLight.color;
		output.rgb = baseColor * cos * gSpotLight.intensity;
		output.rgb += specular;
		output.a = 1.0f;
	}
	// それ以外は異常値なので、黒を出力
	else {
		output = float4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	return output;
}