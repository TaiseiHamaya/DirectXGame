#include "Basis.h"

#include <cmath>

#include "Quaternion.h"
#include "Matrix3x3.h"

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

Matrix3x3 Basis::to_matrix() const {
	Matrix3x3 result;
	for (int i = 0; i < 3; ++i) {
		std::memcpy(result[i].data(), &rows[i], sizeof(float) * 3);
	}
	return result;
}

Matrix4x4 Basis::to_matrix4x4() const {
	Matrix4x4 result;
	for (int i = 0; i < 3; ++i) {
		std::memcpy(result[i].data(), &rows[i], sizeof(float) * 3);
	}
	return result;
}

void Basis::scale_rotate(const Vector3& scale, const Quaternion& rotate) {
	*this = rotate.to_basis();
	rows[0] *= scale.x;
	rows[1] *= scale.y;
	rows[2] *= scale.z;
}

Vector3 Basis::to_scale() const {
	return {
		column(0).length(),
		column(1).length(),
		column(2).length()
	};
}

Quaternion Basis::to_quaternion() const {
	// 理論 : https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
	// Code : https://github.com/godotengine/godot/blob/master/core/math/basis.cpp (GodotEngine Basis::get_quaternion()関数より)

	Vector3 xyz;
	float w;
	float trace = rows[0][0] + rows[1][1] + rows[2][2];
	if (trace > 0.0f) {
		float s = std::sqrt(trace + 1.0f);
		w = s * 0.5f;
		s = 0.5f / s;
		xyz[0] = (rows[2][1] - rows[1][2]) * s;
		xyz[1] = (rows[0][2] - rows[2][0]) * s;
		xyz[2] = (rows[1][0] - rows[0][1]) * s;
	}
	else {
		int i = rows[0][0] < rows[1][1]
			? (rows[1][1] < rows[2][2] ? 2 : 1)
			: (rows[0][0] < rows[2][2] ? 2 : 0);
		int j = (i + 1) % 3;
		int k = (i + 2) % 3;

		float s = std::sqrt(rows[i][i] - rows[j][j] - rows[k][k] + 1.0f);
		xyz[i] = s * 0.5f;
		s = 0.5f / s;

		w = (rows[k][j] - rows[j][k]) * s;
		xyz[j] = (rows[j][i] + rows[i][j]) * s;
		xyz[k] = (rows[k][i] + rows[i][k]) * s;
	}

	return Quaternion{ xyz,w };
}
