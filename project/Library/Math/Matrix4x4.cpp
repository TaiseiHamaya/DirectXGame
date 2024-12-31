#include "Matrix4x4.h"

const Matrix4x4 Matrix4x4::Inverse(const Matrix4x4& matrix) {
	return matrix.inverse();
}

// Gaussian elimination { O(N^3) == 64 }
const Matrix4x4 Matrix4x4::inverse() const {
	constexpr size_t AugmentedColumn = COLUMN * 2;
	Matrix<ROW, AugmentedColumn> augmented;
	for (size_t i = 0; i < ROW; ++i) {
		for (size_t j = 0; j < COLUMN; ++j) {
			augmented[i][j] = _matrix[i][j];
		}
		augmented[i][i + COLUMN] = 1;
	}

	for (size_t i = 0; i < ROW; ++i) {
		// [i][i] = 0だと左側が単位行列にならないので、別の行と交換して解決
		// また、1e-6以下の少数をpivotに選択すると、数値的安定性が下がるので、ここで排除
		if (std::abs(augmented[i][i]) < 1e-6f) {
			bool found = false;
			for (size_t k = i + 1; k < ROW; ++k) {
				if (std::abs(augmented[k][i]) >= 1e-6f) {
					std::swap(augmented[i], augmented[k]);
					found = true;
					break;
				}
			}
			if (!found) {
				// 交換できない場合は何かがおかしいので止める
				assert(false);
			}
		}

		// 単位行列にするため[i][i]を取得
		float pivot = augmented[i][i];
		for (size_t j = 0; j < AugmentedColumn; ++j) {
			augmented[i][j] /= pivot;
		}
		// すべての列について、i != kの場合は0にするため別の行から持ってきてどうにかする
		for (size_t k = 0; k < ROW; ++k) {
			if (k != i) {
				// その他の行のi要素を0にする
				float factor = augmented[k][i];
				if (factor != 0) {
					// 定義より、k行のすべての列をi行目の定数倍する
					for (size_t j = 0; j < AugmentedColumn; ++j) {
						augmented[k][j] -= factor * augmented[i][j];
					}
				}
			}
		}
	}

	// 逆行列を抽出
	Matrix4x4 inversed{};
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			inversed[i][j] = augmented[i][j + 4];
		}
	}

	// 正しくinverseできたかチェック(デバッグのみ)
#ifdef _DEBUG
	bool isInversed = true;
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			if (!(i == j ? augmented[i][j] == 1 : augmented[i][j] == 0)) {
				isInversed = false;
			}
		}
	}
	assert(isInversed);
#endif // _DEBUG

	return inversed;
}
