#include "Tools/Lighing/DirectionalLighting.hlsli"

#include "FullscreenShader.hlsli"

#include "Tools/PackNormalV2.hlsli"
#include "Tools/PackA2.hlsli"
#include "Tools/PackShininess.hlsli"
#include "Tools/ToLinearZBuffer.hlsli"

struct Camera {
	float3 position;
	float4x4 viewInv;
	float4x4 projInv;
};

Texture2D<float4> gAlbedoShading : register(t0);
Texture2D<float4> gNormal : register(t1);
Texture2D<float> gDepth : register(t2);

StructuredBuffer<DirectionalLightBuffer> gDirectionalLight : register(t3);
ConstantBuffer<Camera> gCamera : register(b0);

[earlydepthstencil]
float4 main(VertexShaderOutput input) : SV_TARGET {
	float4 output = float4(0, 0, 0, 0);
	
	// sampling
	float4 albedoShading = gAlbedoShading.Load(input.position.xyz);
	float4 normalViewShininess = gNormal.Load(input.position.xyz);
	float ndcDepth = gDepth.Load(input.position.xyz);
	
	Pixel pixel;
	// unpack
	pixel.color = albedoShading.rgb;
	uint shadingType = UnpackA2bit(albedoShading.a);
	pixel.normal = mul(UnpackingNormaV2(normalViewShininess.xy), (float3x3)gCamera.viewInv);
	pixel.shininess = UnpackShininess(normalViewShininess.zw);
	float3 ndc = float3(input.texcoord.xy * 2 - 1, ndcDepth);
	ndc.y *= -1;
	float4 view = mul(float4(ndc, 1.0f), gCamera.projInv);
	float4 worldH = mul(view, gCamera.viewInv);
	pixel.world = worldH.xyz / worldH.w;
	pixel.color = albedoShading.rgb;
	
	DirectionalLightBuffer directionalLight = gDirectionalLight[input.instance];

	LightingData lightingData = CalcLightingData(pixel, gCamera.position, directionalLight);
	
	// ライティングなし
	if (shadingType == 0) {
		discard;
	}
	// Lambert
	else if (shadingType == 1) {
		output.rgb = BlinnPhongSpecular(lightingData) + LambertDiffuse(lightingData);
		output.a = 1.0f;
	}
	// Half Lambert
	else if (shadingType == 2) {
		output.rgb = BlinnPhongSpecular(lightingData) + HalfLambertDiffuse(lightingData);
		output.a = 1.0f;
	}
	// それ以外は異常値なので、discard
	else {
		discard;
	}
	
	// 透明の場合は出力せず終了
	// サンプリングによっては0にならない場合があるためバッファを取る
	if (output.a <= 0.05f) {
		discard;
	}
	
	return output;
}