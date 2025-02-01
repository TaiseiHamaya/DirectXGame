#include "Misc/PrimitiveGeometry/PrimitiveGeometry.hlsli"
#include "Tools/PackNormalV2.hlsli"
#include "Tools/PackA2.hlsli"
#include "Tools/ToLinearZBuffer.hlsli"

struct PointLight {
	float3 color; // 色
	float intensity; // 輝度
	float3 position; // 位置
	float radius; // 範囲
	float decay; // 減衰率
};

struct Camera {
	float3 position;
	float4x4 viewInv;
	float4x4 projInv;
};

Texture2D<float4> gAlbedoShading : register(t0);
Texture2D<float3> gNormal : register(t1);
Texture2D<float> gDepth : register(t2);
SamplerState gSampler : register(s0);

StructuredBuffer<PointLight> gPointLight : register(t3);
ConstantBuffer<Camera> gCamera : register(b0);
//const bool isFrontFace : SV_IsFrontFace
float4 main(VertexShaderOutput input) : SV_TARGET {
	float4 output;
	
	const float2 texcoord = input.position.xy / float2(1280, 720);
	
	// sampling
	float4 albedoShading = gAlbedoShading.Sample(gSampler, texcoord.xy);
	float3 normalViewShininess = gNormal.Sample(gSampler, texcoord.xy);
	float ndcDepth = gDepth.Sample(gSampler, texcoord.xy);
	
	// unpack
	float3 albedo = albedoShading.rgb;
	uint shadingType = UnpackA2bit(albedoShading.a);
	float3 normal = mul(UnpackingNormaV2(normalViewShininess.xy), (float3x3)gCamera.viewInv);
	float shininess = normalViewShininess.z;
	//float3 normal = normalize(normalShininess.xyz);
	float3 ndc = float3(texcoord * 2 - 1, ndcDepth);
	ndc.y *= -1;
	float4 view = mul(float4(ndc, 1.0f), gCamera.projInv);
	float4 worldH = mul(view, gCamera.viewInv);
	float3 world = worldH.xyz / worldH.w;
	
	PointLight pointLight = gPointLight[input.instance];
	
	// specular
	float3 toCamera = normalize(gCamera.position - world);
	float3 direction = normalize(world - pointLight.position);
	float distance = length(world - pointLight.position);
	
	//if (!isFrontFace && pointLight.radius > distance) {
	//	discard;
	//}
	
	float factor = pow(saturate(-distance / pointLight.radius + 1.0f), pointLight.decay);
	if (isnan(factor)) {
		factor = 0;
	}
	float3 lightColor = pointLight.color.rgb * pointLight.intensity * factor;
	
	float3 halfVector = normalize(-direction + toCamera);
	float dotNormal = dot(normal, halfVector);
	float specularPow = pow(saturate(dotNormal), shininess);
	if (isnan(specularPow)) {
		specularPow = 0;
	}
	else {
		specularPow *= 2.0f;
	}
	float3 specular = lightColor * pointLight.intensity * specularPow * albedo;

	
	// ライティングなし
	if (shadingType == 0) {
		output.xyz = albedo;
		output.a = 1.0f;
	}
	// Lambert
	else if (shadingType == 1) {
		float NdotL = dot(normal, -direction);
		float cos = saturate(NdotL);
		float3 baseColor = albedo * lightColor;
		output.rgb = baseColor * cos * pointLight.intensity;
		output.rgb += specular;
		output.a = 1.0f;
	}
	// Half Lambert
	else if (shadingType == 2) {
		float NdotL = dot(normal, -direction);
		float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
		float3 baseColor = albedo * lightColor;
		output.rgb = baseColor * cos * pointLight.intensity;
		output.rgb += specular;
		output.a = 1.0f;
	}
	// それ以外は異常値なので、黒を出力
	else {
		output = float4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	// 透明の場合は出力せず終了
	// サンプリングによっては0にならない場合があるためバッファを取る
	if (output.a <= 0.05f) {
		discard;
	}
	
	return output;
}