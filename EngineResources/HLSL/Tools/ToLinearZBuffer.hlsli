float ToLinearZBuffer(float ndcZ, float near, float far) {
	return far * near / (far - ndcZ * (far - near));
}
