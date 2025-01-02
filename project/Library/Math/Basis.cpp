#include "Basis.h"

#include <algorithm>

#include "Quaternion.h"

#define cofac(row1, col1, row2, col2) \
	(rows[row1][col1] * rows[row2][col2] - rows[row1][col2] * rows[row2][col1])

void Basis::invert() {
	float co[3] = {
		cofac(1, 1, 2, 2), cofac(1, 2, 2, 0), cofac(1, 0, 2, 1)
	};
	float det = rows[0][0] * co[0] +
		rows[0][1] * co[1] +
		rows[0][2] * co[2];
	float s = 1.0f / det;

	*this = {
		Vector3(co[0] * s, cofac(0, 2, 2, 1) * s, cofac(0, 1, 1, 2) * s),
		Vector3(co[1] * s, cofac(0, 0, 2, 2) * s, cofac(0, 2, 1, 0) * s),
		Vector3(co[2] * s, cofac(0, 1, 2, 0) * s, cofac(0, 0, 1, 1) * s)
	};
}

void Basis::transpose() {
	std::swap(rows[0][1], rows[1][0]);
	std::swap(rows[0][2], rows[2][0]);
	std::swap(rows[1][2], rows[2][1]);
}

Basis Basis::inverse() const {
	Basis inv = *this;
	inv.invert();
	return inv;
}

Basis Basis::transposed() const {
	Basis tr = *this;
	tr.transpose();
	return tr;
}

void Basis::scale_rotate(const Vector3& scale, const Quaternion& rotate) {
	*this = rotate.to_basis();
	rows[0] *= scale.x;
	rows[1] *= scale.y;
	rows[2] *= scale.z;
}
