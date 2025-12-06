#include "Matrix3x3.h"

#include "Engine/Application/Logger.h"

Matrix3x3 Matrix3x3::Inverse(const Matrix3x3& matrix) {
	return matrix.inverse();
}

const Matrix3x3 Matrix3x3::inverse() const {
	r32 length =
		matrix[0][0] * matrix[1][1] * matrix[2][2] +
		matrix[0][1] * matrix[1][2] * matrix[2][0] +
		matrix[0][2] * matrix[1][0] * matrix[2][1] -

		matrix[0][2] * matrix[1][1] * matrix[2][0] -
		matrix[0][1] * matrix[1][0] * matrix[2][2] -
		matrix[0][0] * matrix[1][2] * matrix[2][1];

	szgWarningIf(length == 0, "Matrix3x3::inverse was called. But determinant is 0.");

	Matrix3x3 result{
		{ (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) / length, -(matrix[0][1] * matrix[2][2] - matrix[0][2] * matrix[2][1]) / length, (matrix[0][1] * matrix[1][2] - matrix[0][2] * matrix[1][1]) / length },
		{ -(matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) / length, (matrix[0][0] * matrix[2][2] - matrix[0][2] * matrix[2][0]) / length, -(matrix[0][0] * matrix[1][2] - matrix[0][2] * matrix[1][0]) / length },
		{ (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]) / length, -(matrix[0][0] * matrix[2][1] - matrix[0][1] * matrix[2][0]) / length, (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]) / length },
	};
	return result;
}