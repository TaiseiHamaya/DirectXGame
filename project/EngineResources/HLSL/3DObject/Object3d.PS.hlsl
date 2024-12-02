#include "Object3d.hlsli"

struct Material {
	float4 color;
	int lightingType;
	float shininess;
	float4x4 uvTransform;
};

struct PixelShaderOutput {
	float4 color : SV_Target0;
};

struct SpotLight {
	float4 color; // 色
	float3 position; // 位置
	float intensity; // 輝度
	float3 direction; // 向き
	float distance; // ライト距離
	float decay; // 距離減衰率
	float angle; // ライト範囲
	float falloffStart; // ライト
};

struct CameraInfoPS {
	float3 position;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<SpotLight> gSpotLight : register(b1);
ConstantBuffer<CameraInfoPS> gCameraInfoPS : register(b2);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	float4 baseColor = gMaterial.color * textureColor;
	
	float3 toCamera = normalize(gCameraInfoPS.position - input.world);
	//float3 reflectLight = reflect(gDirectionalLight.directon, normalize(input.normal));
	//float dotReflect = dot(reflectLight, toCamera);
	//float specularPow = pow(saturate(dotReflect), gMaterial.shininess);
	//specularPow = isnan(specularPow) ? 0 : specularPow * 2.0f;
	float3 direction = normalize(input.world - gSpotLight.position);
	// 距離減衰
	float distance = length(input.world - gSpotLight.position);
	float distanceDecayFactor = pow(saturate(-distance / gSpotLight.distance + 1.0f), gSpotLight.decay);
	// 範囲減衰
	float cosAngle = dot(direction, gSpotLight.direction);
	float falloffFactor = saturate((cosAngle - gSpotLight.angle) / (gSpotLight.falloffStart - gSpotLight.angle));
	// ライト色
	float3 lightColor = gSpotLight.color.rgb * gSpotLight.intensity * distanceDecayFactor * falloffFactor;
	
	float3 halfVector = normalize(-direction + toCamera);
	float dotNormal = dot(normalize(input.normal), halfVector);
	float specularPow = pow(saturate(dotNormal), gMaterial.shininess);
	if (isnan(specularPow)) {
		specularPow = 0;
	}
	else {
		//specularPow *= 2.0f;
	}
	float3 specular = lightColor * specularPow * baseColor.rgb;
	
	// ライティングなし
	if (gMaterial.lightingType == 0) {
		output.color = baseColor;
	}
	// Lambert
	else if (gMaterial.lightingType == 1) {
		float NdotL = dot(normalize(input.normal), -direction);
		float cos = saturate(NdotL);
		output.color.rgb = baseColor.rgb * lightColor * cos;
		output.color.rgb += specular;
		output.color.a = gMaterial.color.a * textureColor.a;
	}
	// Half Lambert
	else if (gMaterial.lightingType == 2) {
		float NdotL = dot(normalize(input.normal), -direction);
		float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
		output.color.rgb = baseColor.rgb * lightColor * cos;
		output.color.rgb += specular;
		output.color.a = baseColor.a;
	}
	// それ以外は異常値なので、白を出力
	else {
		output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	// 透明の場合は出力せず終了
	// サンプリングによっては0にならない場合があるためバッファを取る
	if (output.color.a <= 0.05f) {
		discard;
	}
	return output;
}