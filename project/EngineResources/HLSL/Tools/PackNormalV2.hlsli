float2 PackingNormalV2(float3 viewNormal) {
	return normalize(viewNormal).xy * 0.5f + 0.5f;
}

float3 UnpackingNormaV2(float2 viewNormalPacked) {
	float2 viewNormal = viewNormalPacked * 2.0f - 1.0f;
	float z = -sqrt(1.0f - saturate(dot(viewNormal, viewNormal)));
	return normalize(float3(viewNormal, z));
}
