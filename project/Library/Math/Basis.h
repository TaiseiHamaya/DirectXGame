#pragma once

#include "Vector3.h"

#include <cstdint>
#include <array>

class Quaternion;
class Matrix3x3;
class Matrix4x4;

class Basis final {
private:
	std::array<Vector3, 3> rows = {
		CVector3::BASIS_X,
		CVector3::BASIS_Y,
		CVector3::BASIS_Z,
	};

public:
	constexpr Basis() = default;
	~Basis() = default;

	constexpr Basis(const Vector3& rowX, const Vector3& rowY, const Vector3& rowZ);

public:
	constexpr const Vector3& operator[](uint32_t row) const;
	constexpr Vector3& operator[](uint32_t row);
	constexpr Basis operator*(const Basis& rhs) const;
	constexpr Basis& operator*=(const Basis& rhs);

	constexpr friend Vector3 operator*(const Vector3& v, const Basis& basis);

public:
	void invert();
	void transpose();

	Basis inverse() const;
	Basis transposed() const;

	Matrix3x3 to_matrix() const;
	Matrix4x4 to_matrix4x4() const;

	constexpr Vector3 column(uint32_t index) const noexcept;

	void scale_rotate(const Vector3& scale, const Quaternion& rotate);

public:
	Vector3 to_scale() const;
	Quaternion to_quaternion() const;
};

inline constexpr Basis::Basis(const Vector3& rowX, const Vector3& rowY, const Vector3& rowZ) :
	rows({ rowX, rowY, rowZ }) {
}

inline constexpr const Vector3& Basis::operator[](uint32_t row) const {
	return rows[row];
}

constexpr Vector3& Basis::operator[](uint32_t row) {
	return rows[row];
}

inline constexpr Basis Basis::operator*(const Basis& rhs) const {
	return{
		rows[0] * rhs,
		rows[1] * rhs,
		rows[2] * rhs
	};
}

inline constexpr Basis& Basis::operator*=(const Basis& rhs) {
	*this = *this * rhs;
	return *this;
}

inline constexpr Vector3 operator*(const Vector3& v, const Basis& basis) {
	const Vector3 rColumn0 = basis.column(0);
	const Vector3 rColumn1 = basis.column(1);
	const Vector3 rColumn2 = basis.column(2);
	return {
		Vector3::DotProduct(v, rColumn0),
		Vector3::DotProduct(v, rColumn1),
		Vector3::DotProduct(v, rColumn2)
	};
}

constexpr Vector3 Basis::column(uint32_t index) const noexcept {
	return { rows[0][index],rows[1][index] ,rows[2][index] };
}
