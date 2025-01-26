float3 ToUnormNormal(float3 normal) {
	return normalize(normal) * 0.5f + 0.5f;
}

float3 FromUnormNormal(float3 normal) {
	return normalize((normal - 0.5f) * 2.0f);
}
