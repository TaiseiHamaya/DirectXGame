#pragma once

#include <cassert>
#include <Engine/Math/Array2D.h>

template<size_t ROW, size_t COLUMN>
class Matrix {
	static_assert(ROW > 0);
	static_assert(COLUMN > 0);
public:// ------------------デフォルトコンストラクタ------------------
	Matrix(void) noexcept = default;
	~Matrix(void) = default;

public: // ------------------コンストラクタ------------------
	Matrix(const Matrix<ROW, COLUMN>& rhs) noexcept = default;
	Matrix(Matrix<ROW, COLUMN>&& rhs) noexcept = default;

	inline const Matrix<ROW, COLUMN>& operator=(const Matrix<ROW, COLUMN>& rhs) noexcept {
		_matrix = rhs._matrix;
		return *this;
	}

private:// ------------------行列------------------
	Array2D<float, ROW, COLUMN> _matrix;

public: // ------------------アクセッサ------------------
	const size_t row() const { return _matrix.row(); };
	const size_t column() const { return _matrix.column(); };
	size_t row() { return _matrix.row(); };
	size_t column() { return _matrix.column(); };

public:
	static const Matrix<ROW, COLUMN> Multiply(const Matrix<ROW, COLUMN>& matrix, const float& times);

public:
	const Matrix<ROW, COLUMN> transpose() const;

public: // ------------------演算子オーバーロード------------------
	std::array<float, COLUMN>& operator[](size_t index) { return _matrix[index]; }
	const std::array<float, COLUMN>& operator[](size_t index) const { return _matrix[index]; }
	bool operator==(const Matrix<ROW, COLUMN>& rhs) const { return _matrix == rhs._matrix; }
	bool operator!=(const Matrix<ROW, COLUMN>& rhs) const { return !(*this == rhs); }


	inline const Matrix<ROW, COLUMN> operator+(const Matrix<ROW, COLUMN>& opr) const {
		Matrix<ROW, COLUMN> result;
		for (std::size_t i = 0; i < ROW; i++) {
			for (std::size_t j = 0; j < COLUMN; j++) {
				result[i][j] = _matrix[i][j] + opr[i][j];
			}
		}
		return result;
	}

	inline const Matrix<ROW, COLUMN> operator-(const Matrix<ROW, COLUMN>& opr) const {
		Matrix<ROW, COLUMN> result;
		for (std::size_t i = 0; i < ROW; i++) {
			for (std::size_t j = 0; j < COLUMN; j++) {
				result[i][j] = _matrix[i][j] - opr[i][j];
			}
		}
		return result;
	}

	template<size_t RHS_COLUMN>
	inline Matrix<ROW, COLUMN> operator*(const Matrix<COLUMN, RHS_COLUMN>& opr) const {
		assert(column() == opr.row());
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

	inline Matrix<ROW, COLUMN>& operator+=(const Matrix<ROW, COLUMN>& opr) {
		for (std::size_t i = 0; i < ROW; i++) {
			for (std::size_t j = 0; j < COLUMN; j++) {
				_matrix[i][j] += opr[i][j];
			}
		}
		return *this;
	}

	template<size_t RHS_COLUMN>
	inline Matrix<ROW, COLUMN>& operator*=(const Matrix<COLUMN, RHS_COLUMN>& opr) {
		assert(column() == opr.row());
		*this = (*this) * opr;
		return *this;
	}
};

// ------------------3x3の特殊行列を継承定義------------------
class Matrix3x3 final {
public: // コンストラクタ
	Matrix3x3() noexcept = default;
	Matrix3x3(const Matrix3x3& rhs) noexcept = default;
	Matrix3x3(Matrix3x3&& rhs) noexcept = default;
	Matrix3x3(const Matrix<3, 3>&& rhs) noexcept;
	Matrix3x3(const std::initializer_list<std::initializer_list<float>>& init);

public: // static関数
	static Matrix3x3 Multiply(const Matrix3x3& matrix, const float& times);

	static Matrix3x3 Inverse(const Matrix3x3& matrix);

	static Matrix3x3 Transpose(const Matrix3x3& matrix);

public: // メンバ関数
	const Matrix3x3 inverse() const;
	const Matrix3x3 transpose() const;

private:// has継承
	Matrix<3, 3> __matrix;

public:
	// ------------------演算子オーバーロード------------------
	inline const Matrix3x3& operator=(const Matrix3x3& opr) {
		__matrix = opr.__matrix;
		return *this;
	}

	const std::array<float, 3>& operator[](std::size_t index) const {
		return __matrix[index];
	}

	std::array<float, 3>& operator[](std::size_t index) {
		return __matrix[index];
	}

	inline const Matrix3x3 operator+(const Matrix3x3& opr) const {
		return (__matrix + opr.__matrix);
	}

	inline Matrix3x3 operator*(const Matrix3x3& opr) const {
		return (__matrix * opr.__matrix);
	}

	inline Matrix3x3& operator+=(const Matrix3x3& opr) {
		*this = (__matrix + opr.__matrix);
		return *this;
	}

	inline Matrix3x3& operator*=(const Matrix3x3& opr) {
		*this = (__matrix * opr.__matrix);
		return *this;
	}

public:
	static const Matrix3x3 identity;
};

// ------------------3x3の特殊行列を継承定義------------------
class Matrix4x4 final {
public: // コンストラクタ
	Matrix4x4() noexcept = default;
	Matrix4x4(const Matrix4x4& rhs) noexcept = default;
	Matrix4x4(Matrix4x4&& rhs) noexcept = default;
	Matrix4x4(const Matrix<4, 4>& rhs);
	Matrix4x4(std::initializer_list<std::initializer_list<float>> init);

public: // static関数
	static Matrix4x4 Multiply(const Matrix4x4& matrix, const float& times);

	static Matrix4x4 Inverse(const Matrix4x4& matrix);

	static Matrix4x4 Transpose(const Matrix4x4& matrix);

	static Matrix4x4 Convert3x3(const Matrix3x3& matrix);

public: // メンバ関数
	const Matrix4x4 inverse() const;
	const Matrix4x4 transpose() const;

private:// has継承
	Matrix<4, 4> __matrix;

public:
	// ------------------演算子オーバーロード------------------
	inline const Matrix4x4& operator=(const Matrix4x4& opr) {
		__matrix = opr.__matrix;
		return *this;
	}

	const std::array<float, 4>& operator[](std::size_t index) const {
		return __matrix[index];
	}

	std::array<float, 4>& operator[](std::size_t index) {
		return __matrix[index];
	}

	inline const Matrix4x4 operator+(const Matrix4x4& opr) const {
		return (__matrix + opr.__matrix);
	}

	inline const Matrix4x4 operator-(const Matrix4x4& opr) const {
		return (__matrix - opr.__matrix);
	}

	inline Matrix4x4 operator*(const Matrix4x4& opr) const {
		return (__matrix * opr.__matrix);
	}

	inline Matrix4x4& operator+=(const Matrix4x4& opr) {
		*this = (__matrix + opr.__matrix);
		return *this;
	}

	inline Matrix4x4& operator*=(const Matrix4x4& opr) {
		*this = (__matrix * opr.__matrix);
		return *this;
	}

	inline bool operator==(const Matrix4x4& rhs) const {
		return __matrix == rhs.__matrix;
	}

	inline bool operator!=(const Matrix4x4& rhs) const {
		return !(*this == rhs);
	}

	/// <summary>
	/// 静的メンバ変数
	/// </summary>
public:
	static const Matrix4x4 identity;
};

template<size_t ROW, size_t COLUMN>
inline const Matrix<ROW, COLUMN> Matrix<ROW, COLUMN>::Multiply(const Matrix<ROW, COLUMN>& matrix, const float& times) {
	Matrix<ROW, COLUMN> result;
	for (int i = 0; i < ROW; ++i) {
		for (int j = 0; j < COLUMN; ++j) {
			result[i][j] = matrix[i][j] * times;
		}
	}
	return result;
}

template<size_t ROW, size_t COLUMN>
inline const Matrix<ROW, COLUMN> Matrix<ROW, COLUMN>::transpose() const {
	Matrix<ROW, COLUMN> result;
	for (int i = 0; i < ROW; ++i) {
		for (int j = 0; j < COLUMN; ++j) {
			result._matrix[i][j] = this->_matrix[j][i];
		}
	}
	return result;
}
