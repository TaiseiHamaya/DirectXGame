float3 transform(float3 position, float4x3 transform) {
	return mul(position, (float3x3)transform) + transform[3];
};
