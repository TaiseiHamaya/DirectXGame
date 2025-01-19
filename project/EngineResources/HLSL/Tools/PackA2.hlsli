uint UnpackA2bit(float a2) {
	return a2 * 3;
};

float PackA2bit(uint a2) {
	return a2 / 3.0f;
}
