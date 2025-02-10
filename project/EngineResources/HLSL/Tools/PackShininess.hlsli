float2 PackShininess(float shininess) {
	float temp = clamp(shininess, 0, 399.999) / 100;
	return float2(
		frac(temp),
		trunc(temp) / 3.0f
	);
}

float UnpackShininess(float2 packed) {
	return float(packed.y * 3.0f + packed.x) * 100;
}
