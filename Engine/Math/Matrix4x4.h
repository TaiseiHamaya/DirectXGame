#pragma once

#include "Matrix.h"
#include <cassert>

class Matrix3x3;

// ------------------3x3の特殊行列を継承定義------------------
class Matrix4x4 final {
public: // コンストラクタ
	constexpr Matrix4x4() noexcept = default;
	constexpr Matrix4x4(const Matrix4x4& rhs) noexcept = default;
	constexpr Matrix4x4(Matrix4x4&& rhs) noexcept = default;
	constexpr Matrix4x4(const Matrix<4, 4>& rhs);
	constexpr Matrix4x4(const std::initializer_list<std::initializer_list<float>>& init);

public:
	// ------------------演算子オーバーロード------------------
	constexpr Matrix4x4& operator=(const Matrix4x4&) noexcept = default;
	constexpr Matrix4x4& operator=(Matrix4x4&&) noexcept = default;

	constexpr const std::array<float, 4>& operator[](std::size_t index) const;
	constexpr std::array<float, 4>& operator[](std::size_t index);

	constexpr const Matrix4x4 operator+(const Matrix4x4& opr) const;
	constexpr const Matrix4x4 operator-(const Matrix4x4& opr) const;
	constexpr const Matrix4x4 operator*(const Matrix4x4& opr) const;
	constexpr Matrix4x4& operator+=(const Matrix4x4& opr);
	constexpr Matrix4x4& operator*=(const Matrix4x4& opr);

	constexpr bool operator==(const Matrix4x4& rhs) const noexcept;
	constexpr bool operator!=(const Matrix4x4& rhs) const noexcept;

public: // static関数
	/// <summary>
	/// 定数倍
	/// </summary>
	/// <param name="matrix">元Matrix</param>
	/// <param name="times">倍数</param>
	/// <returns>計算値</returns>
	static constexpr Matrix4x4 Multiply(const Matrix4x4& matrix, const float& times);

	/// <summary>
	/// 逆行列計算
	/// </summary>
	/// <param name="matrix">計算元行列</param>
	/// <returns>変換後行列</returns>
	static const Matrix4x4 Inverse(const Matrix4x4& matrix);

	/// <summary>
	/// 転置行列
	/// </summary>
	/// <param name="matrix">計算元行列</param>
	/// <returns>変換後行列</returns>
	static constexpr Matrix4x4 Transpose(const Matrix4x4& matrix);

	/// <summary>
	/// 3x3行列を4x4に変換(埋める値は全て0)
	/// </summary>
	/// <param name="matrix">3x3行列</param>
	/// <returns>変換後行列</returns>
	static constexpr Matrix4x4 Convert3x3(const Matrix3x3& matrix);

public: // メンバ関数
	/// <summary>
	/// 逆行列計算
	/// </summary>
	/// <returns>変換後行列</returns>
	const Matrix4x4 inverse() const;

	/// <summary>
	/// 転置行列
	/// </summary>
	/// <returns>変換後行列</returns>
	constexpr const Matrix4x4 transpose() const;

private:// has継承
	Matrix<4, 4> __matrix;
};

constexpr Matrix4x4::Matrix4x4(const Matrix<4, 4>& rhs) {
	__matrix = rhs;
}

constexpr Matrix4x4::Matrix4x4(const std::initializer_list<std::initializer_list<float>>& init) {
	assert(init.size() == 4 && init.begin()->size() == 4);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			__matrix[i][j] = *((init.begin() + i)->begin() + j);
		}
	}
}

constexpr const std::array<float, 4>& Matrix4x4::operator[](std::size_t index) const {
	return __matrix[index];
}

constexpr std::array<float, 4>& Matrix4x4::operator[](std::size_t index) {
	return __matrix[index];
}

constexpr const Matrix4x4 Matrix4x4::operator+(const Matrix4x4& opr) const {
	return (__matrix + opr.__matrix);
}

constexpr const Matrix4x4 Matrix4x4::operator-(const Matrix4x4& opr) const {
	return (__matrix - opr.__matrix);
}

constexpr const Matrix4x4 Matrix4x4::operator*(const Matrix4x4& opr) const {
	return (__matrix * opr.__matrix);
}

constexpr Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& opr) {
	*this = (__matrix + opr.__matrix);
	return *this;
}

constexpr Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& opr) {
	*this = (__matrix * opr.__matrix);
	return *this;
}

constexpr bool Matrix4x4::operator==(const Matrix4x4& rhs) const noexcept {
	return __matrix == rhs.__matrix;
}

constexpr bool Matrix4x4::operator!=(const Matrix4x4& rhs) const noexcept {
	return !(*this == rhs);
}

constexpr Matrix4x4 Matrix4x4::Multiply(const Matrix4x4& matrix, const float& times) {
	return Matrix<4, 4>::Multiply(matrix.__matrix, times);
}

constexpr Matrix4x4 Matrix4x4::Transpose(const Matrix4x4& matrix) {
	return matrix.transpose();
}

#include "Matrix3x3.h"
constexpr Matrix4x4 Matrix4x4::Convert3x3(const Matrix3x3& matrix) {
	Matrix4x4 result{};
	for (uint32_t r = 0; r < 3; ++r) {
		for (uint32_t c = 0; c < 3; ++c) {
			result[r][c] = matrix[r][c];
		}
	}
	return result;
}

constexpr const Matrix4x4 Matrix4x4::transpose() const {
	return __matrix.transpose();
}

namespace CMatrix4x4 {
	constexpr Matrix4x4 IDENTITY = {
		{{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{0,0,0,1}}
	};
}