#include "CircleGauge3D.hlsli"

struct Material {
	float4 color;
	int lightingType;
	float4x4 uvTransform;
};

struct Percentage {
	float value;
};

struct PixelShaderOutput {
	float4 color : SV_Target0;
};

static const float PI = acos(-1.0f);
static const float PI2 = PI * 2;

float cross(float2 v1, float2 v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<Percentage> gPercentage : register(b1);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	
	float2 distance = (input.texcoord - float2(0.5f, 0.5f)) * 2;
	if (length(distance) < 1e-4f) {
		distance = float2(0.0f, -1.0f);
	}
	float dotValue = dot(float2(0.0f, -1.0f), normalize(distance));
	float crossValue = cross(float2(0.0f, -1.0f), normalize(distance));
	float angle = atan2(crossValue, dotValue);
	if (angle < 0) {
		angle += PI2;
	}
	// 円の外側はdiscard
	if (length(distance) > 1.0f) {
		discard;
	}
	// 範囲内であれば描画
	else if (angle < gPercentage.value * PI2) {
		float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
		float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
		output.color = gMaterial.color * textureColor;
		// 透明の場合は出力せず終了
		// サンプリングによっては0にならない場合があるためバッファを取る
		if (output.color.a <= 0.05f) {
			discard;
		}
	}
	// 範囲外なのでdiscard
	else {
		discard;
	}
	return output;
}