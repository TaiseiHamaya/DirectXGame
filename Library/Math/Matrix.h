#pragma once

#include <Library/Utility/Template/Array2D.h>

template<size_t ROW, size_t COLUMN>
class Matrix {
	static_assert(ROW > 0, "Matrix's ROW must be more than 0");
	static_assert(COLUMN > 0, "Matrix's COLUMN must be more than 0");
public:// ------------------デフォルトコンストラクタ------------------
	constexpr Matrix(void) noexcept = default;
	constexpr ~Matrix(void) = default;

public: // ------------------コンストラクタ------------------
	constexpr Matrix(const Matrix<ROW, COLUMN>& rhs) noexcept = default;
	constexpr Matrix(Matrix<ROW, COLUMN>&& rhs) noexcept = default;

	constexpr Matrix<ROW, COLUMN>& operator=(const Matrix<ROW, COLUMN>& rhs) noexcept = default;
	constexpr Matrix<ROW, COLUMN>& operator=(Matrix<ROW, COLUMN>&& rhs) noexcept = default;

private:// ------------------行列------------------
	Array2D<float, ROW, COLUMN> _matrix;

public: // ------------------アクセッサ------------------
	constexpr const size_t row() const noexcept { return _matrix.row(); };
	constexpr const size_t column() const noexcept { return _matrix.column(); };
	constexpr size_t row() noexcept { return _matrix.row(); };
	constexpr size_t column() noexcept { return _matrix.column(); };

public:
	static constexpr Matrix<ROW, COLUMN> Multiply(const Matrix<ROW, COLUMN>& matrix, const float& times) noexcept(false);

public:
	constexpr Matrix<COLUMN, ROW> transpose() const noexcept(false);

public: // ------------------演算子オーバーロード------------------
	constexpr std::array<float, COLUMN>& operator[](size_t index) noexcept(false) { return _matrix[index]; }
	constexpr const std::array<float, COLUMN>& operator[](size_t index) const noexcept(false) { return _matrix[index]; }
	constexpr bool operator==(const Matrix<ROW, COLUMN>& rhs) const noexcept { return _matrix == rhs._matrix; }
	constexpr bool operator!=(const Matrix<ROW, COLUMN>& rhs) const noexcept { return !(*this == rhs); }

	constexpr const Matrix<ROW, COLUMN> operator+(const Matrix<ROW, COLUMN>& opr) const noexcept(false);
	constexpr const Matrix<ROW, COLUMN> operator-(const Matrix<ROW, COLUMN>& opr) const noexcept(false);
	template<size_t RHS_COLUMN>
	constexpr Matrix<ROW, COLUMN> operator*(const Matrix<COLUMN, RHS_COLUMN>& opr) const noexcept(false);
	constexpr Matrix<ROW, COLUMN>& operator+=(const Matrix<ROW, COLUMN>& opr) noexcept(false);
	template<size_t RHS_COLUMN>
	constexpr Matrix<ROW, COLUMN>& operator*=(const Matrix<COLUMN, RHS_COLUMN>& opr) noexcept(false);
};

template<size_t ROW, size_t COLUMN>
inline constexpr Matrix<ROW, COLUMN> Matrix<ROW, COLUMN>::Multiply(const Matrix<ROW, COLUMN>& matrix, const float& times)  noexcept(false) {
	Matrix<ROW, COLUMN> result;
	for (int i = 0; i < ROW; ++i) {
		for (int j = 0; j < COLUMN; ++j) {
			result[i][j] = matrix[i][j] * times;
		}
	}
	return result;
}

template<size_t ROW, size_t COLUMN>
inline constexpr Matrix<COLUMN, ROW> Matrix<ROW, COLUMN>::transpose() const noexcept(false) {
	Matrix<ROW, COLUMN> result;
	for (int i = 0; i < ROW; ++i) {
		for (int j = 0; j < COLUMN; ++j) {
			result._matrix[i][j] = this->_matrix[j][i];
		}
	}
	return result;
}

template<size_t ROW, size_t COLUMN>
inline constexpr const Matrix<ROW, COLUMN> Matrix<ROW, COLUMN>::operator+(const Matrix<ROW, COLUMN>& opr) const noexcept(false) {
	Matrix<ROW, COLUMN> result;
	for (std::size_t i = 0; i < ROW; i++) {
		for (std::size_t j = 0; j < COLUMN; j++) {
			result[i][j] = _matrix[i][j] + opr[i][j];
		}
	}
	return result;
}

template<size_t ROW, size_t COLUMN>
inline constexpr const Matrix<ROW, COLUMN> Matrix<ROW, COLUMN>::operator-(const Matrix<ROW, COLUMN>& opr) const noexcept(false) {
	Matrix<ROW, COLUMN> result;
	for (std::size_t i = 0; i < ROW; i++) {
		for (std::size_t j = 0; j < COLUMN; j++) {
			result[i][j] = _matrix[i][j] - opr[i][j];
		}
	}
	return result;
}

template<size_t ROW, size_t COLUMN>
inline constexpr Matrix<ROW, COLUMN>& Matrix<ROW, COLUMN>::operator+=(const Matrix<ROW, COLUMN>& opr) noexcept(false) {
	for (std::size_t i = 0; i < ROW; i++) {
		for (std::size_t j = 0; j < COLUMN; j++) {
			_matrix[i][j] += opr[i][j];
		}
	}
	return *this;
}

template<size_t ROW, size_t COLUMN>
template<size_t RHS_COLUMN>
inline constexpr Matrix<ROW, COLUMN> Matrix<ROW, COLUMN>::operator*(const Matrix<COLUMN, RHS_COLUMN>& opr) const noexcept(false) {
	Matrix<ROW, COLUMN> result;
	for (std::size_t i = 0; i < ROW; i++) {
		for (std::size_t k = 0; k < COLUMN; k++) {
			for (std::size_t j = 0; j < COLUMN; j++) {
				result[i][j] += _matrix[i][k] * opr[k][j];
			}
		}
	}
	return result;
}

template<size_t ROW, size_t COLUMN>
template<size_t RHS_COLUMN>
inline constexpr Matrix<ROW, COLUMN>& Matrix<ROW, COLUMN>::operator*=(const Matrix<COLUMN, RHS_COLUMN>& opr) noexcept(false) {
	*this = (*this) * opr;
	return *this;
}
