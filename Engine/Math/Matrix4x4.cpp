#include "Matrix4x4.h"

#include <vector>

const Matrix4x4 Matrix4x4::Inverse(const Matrix4x4& matrix) {
	return matrix.inverse();
}

// Gaussian elimination { O(N^3) == 64 }
const Matrix4x4 Matrix4x4::inverse() const {
	float pivot;
	static constexpr size_t rowSize = 4;
	std::vector<std::vector<float>> augmented(rowSize, std::vector<float>(rowSize * 2, 0));
	for (size_t i = 0; i < rowSize; ++i) {
		for (size_t j = 0; j < rowSize; ++j) {
			augmented[i][j] = __matrix[i][j];
		}
		augmented[i][i + rowSize] = 1;
	}

	for (size_t i = 0; i < rowSize; ++i) {
		pivot = augmented[i][i];
		assert(pivot != 0); // ここで落ちるなら逆行列がない
		for (size_t j = 0; j < rowSize * 2; ++j) {
			augmented[i][j] /= pivot;
		}
		for (size_t k = 0; k < rowSize; ++k) {
			if (k != i) {
				float factor = augmented[k][i];
				if (factor != 0) {
					for (size_t j = 0; j < rowSize * 2; ++j) {
						augmented[k][j] -= factor * augmented[i][j];
					}
				}
			}
		}
	}

	Matrix4x4 inversed{};
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			inversed[i][j] = augmented[i][j + 4];
		}
	}

	// 正しくinverseできたかチェック
#ifdef _DEBUG
	bool isInversed = false;
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			if (!(i == j ? augmented[i][j] == 1 : augmented[i][j] == 0)) {
				isInversed = true;
			}
		}
	}
	assert(!isInversed);
#endif // _DEBUG

	return inversed;
}
