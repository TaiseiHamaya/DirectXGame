#pragma once

#include "Vector3.h"

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
	constexpr const Vector3& operator[](u32 row) const;
	constexpr Vector3& operator[](u32 row);
	constexpr Basis operator*(const Basis& rhs) const;
	constexpr Basis& operator*=(const Basis& rhs);

	constexpr friend Vector3 operator*(const Vector3& v, const Basis& basis);

public:
	void invert();
	void transpose();

	Basis inverse() const;
	Basis transposed() const;

	Basis orthonormalize() const;

	Matrix3x3 to_matrix() const;
	Matrix4x4 to_matrix4x4() const;

	constexpr Vector3 column(u32 index) const noexcept;

	void scale_rotate(const Vector3& scale, const Quaternion& rotate);

public:
	Vector3 to_scale() const;
	Quaternion to_quaternion() const;

public:
	static Basis FromScale(const Vector3& scale);
	static Basis FromRotation(const Quaternion& rotation);
};

inline constexpr Basis::Basis(const Vector3& rowX, const Vector3& rowY, const Vector3& rowZ) :
	rows({ rowX, rowY, rowZ }) {
}

inline constexpr const Vector3& Basis::operator[](u32 row) const {
	return rows[row];
}

constexpr Vector3& Basis::operator[](u32 row) {
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
		Vector3::Dot(v, rColumn0),
		Vector3::Dot(v, rColumn1),
		Vector3::Dot(v, rColumn2)
	};
}

constexpr Vector3 Basis::column(u32 index) const noexcept {
	return { rows[0][index],rows[1][index] ,rows[2][index] };
}
