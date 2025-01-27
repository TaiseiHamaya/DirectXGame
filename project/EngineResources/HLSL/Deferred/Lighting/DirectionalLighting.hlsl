#include "FullscreenShader.hlsli"
#include "Tools/PackNormalV2.hlsli"
#include "Tools/PackA2.hlsli"
#include "Tools/ToLinearZBuffer.hlsli"

struct DirectionalLight {
	float3 color; // 色
	float intensity; // 輝度
	float3 directon; // 向き
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

StructuredBuffer<DirectionalLight> gDirectionalLight : register(t3);
ConstantBuffer<Camera> gCamera : register(b0);

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
	float3 ndc = float3(input.texcoord.xy * 2 - 1, ndcDepth);
	ndc.y *= -1;
	float4 view = mul(float4(ndc, 1.0f), gCamera.projInv);
	float4 worldH = mul(view, gCamera.viewInv);
	float3 world = worldH.xyz / worldH.w;
	
	DirectionalLight directionalLight = gDirectionalLight[input.instance];
	
	// specular
	float3 toCamera = normalize(gCamera.position - world);
	float3 halfVector = normalize(-directionalLight.directon + toCamera);
	float dotNormal = dot(normal, halfVector);
	float specularPow = pow(saturate(dotNormal), shininess);
	if (isnan(specularPow)) {
		specularPow = 0;
	}
	else {
		specularPow *= 2.0f;
	}
	float3 specular = directionalLight.color * directionalLight.intensity * specularPow * albedo;

	
	// ライティングなし
	if (shadingType == 0) {
		output.xyz = albedo;
		output.a = 1.0f;
	}
	// Lambert
	else if (shadingType == 1) {
		float NdotL = dot(normal, -directionalLight.directon);
		float cos = saturate(NdotL);
		float3 baseColor = albedo * directionalLight.color;
		output.rgb = baseColor * cos * directionalLight.intensity;
		output.rgb += specular;
		output.a = 1.0f;
	}
	// Half Lambert
	else if (shadingType == 2) {
		float NdotL = dot(normal, -directionalLight.directon);
		float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
		float3 baseColor = albedo * directionalLight.color;
		output.rgb = baseColor * cos * directionalLight.intensity;
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