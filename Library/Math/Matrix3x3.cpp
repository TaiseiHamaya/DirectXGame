#include "Matrix3x3.h"

#include "Engine/Application/Output.h"

Matrix3x3 Matrix3x3::Inverse(const Matrix3x3& matrix) {
	return matrix.inverse();
}

const Matrix3x3 Matrix3x3::inverse() const {
	r32 length =
		__matrix[0][0] * __matrix[1][1] * __matrix[2][2] +
		__matrix[0][1] * __matrix[1][2] * __matrix[2][0] +
		__matrix[0][2] * __matrix[1][0] * __matrix[2][1] -

		__matrix[0][2] * __matrix[1][1] * __matrix[2][0] -
		__matrix[0][1] * __matrix[1][0] * __matrix[2][2] -
		__matrix[0][0] * __matrix[1][2] * __matrix[2][1];

	WarningIf(length == 0, "Matrix3x3::inverse was called. But determinant is 0.");

	Matrix3x3 result{
		{ (__matrix[1][1] * __matrix[2][2] - __matrix[1][2] * __matrix[2][1]) / length, -(__matrix[0][1] * __matrix[2][2] - __matrix[0][2] * __matrix[2][1]) / length, (__matrix[0][1] * __matrix[1][2] - __matrix[0][2] * __matrix[1][1]) / length },
		{ -(__matrix[1][0] * __matrix[2][2] - __matrix[1][2] * __matrix[2][0]) / length, (__matrix[0][0] * __matrix[2][2] - __matrix[0][2] * __matrix[2][0]) / length, -(__matrix[0][0] * __matrix[1][2] - __matrix[0][2] * __matrix[1][0]) / length },
		{ (__matrix[1][0] * __matrix[2][1] - __matrix[1][1] * __matrix[2][0]) / length, -(__matrix[0][0] * __matrix[2][1] - __matrix[0][1] * __matrix[2][0]) / length, (__matrix[0][0] * __matrix[1][1] - __matrix[0][1] * __matrix[1][0]) / length },
	};
	return result;
}