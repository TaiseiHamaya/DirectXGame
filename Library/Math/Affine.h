#pragma once

#include "Basis.h"
#include "Vector3.h"

class Matrix4x4;
class Transform3D;

class Affine {
private:
	Basis basis;
	Vector3 origin;

public:
	Affine operator*(const Affine& rhs) const;
	Affine& operator*=(const Affine& rhs);

	friend constexpr Vector3 operator*(const Vector3& v, const Affine& affine);

public:
	static Affine FromTransform3D(const Transform3D& transform);
	static Affine FromSRT(const Vector3& scale, const Quaternion& rotation, const Vector3& translate);

public:
	constexpr Vector3 transform(const Vector3& v) const;
	void invert_fast();
	Affine inverse_fast() const;
	void invert();
	Affine inverse() const;

public:
	constexpr const Vector3& get_origin() const { return origin; };
	constexpr const Basis& get_basis() const { return basis; };

public:
	Matrix4x4 to_matrix() const;
};

constexpr Vector3 operator*(const Vector3& v, const Affine& affine) {
	return v * affine.basis + affine.origin;
}

constexpr Vector3 Affine::transform(const Vector3& v) const {
	return v * *this;
}

namespace CAffine {

constexpr Affine IDENTITY{};

}
