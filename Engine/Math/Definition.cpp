#include "Definition.h"

const Vector2 operator*(const Vector2& vec, const Matrix<2, 2>& mat) {
	assert(mat.column() == 2);
	Matrix<1, 2> temp;
	temp[0][0] = vec.x;
	temp[0][1] = vec.y;
	Vector2 result;
	temp *= mat;
	result = { temp[0][0], temp[0][1] };
	return result;
}

Animation::Animation() {
	distance = 1;
	total_seq = 1;
	is_loop = false;
}

Animation::Animation(int distance_, int total_seq_, bool is_loop_) {
	assert(distance_ >= 1);
	assert(total_seq_ >= 1);
	distance = distance_;
	total_seq = total_seq_;
	is_loop = is_loop_;
}
