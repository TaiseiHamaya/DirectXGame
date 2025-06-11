#include "Affine.h"

#include "Matrix4x4.h"
#include "Transform3D.h"

Affine Affine::operator*(const Affine& rhs) const {
	Affine result = *this;
	result *= rhs;
	return result;
}

Affine& Affine::operator*=(const Affine& rhs) {
	origin = origin * rhs;
	basis *= rhs.basis;
	return *this;
}

Affine Affine::FromTransform3D(const Transform3D& transform) {
	return FromSRT(transform.get_scale(), transform.get_quaternion(), transform.get_translate());
}

Affine Affine::FromSRT(const Vector3& scale, const Quaternion& rotation, const Vector3& translate) {
	Affine result;

	result.basis.scale_rotate(scale, rotation);
	result.origin = translate;
	return result;
}

Affine Affine::FromScale(const Vector3& scale) {
	Affine result;

	result.basis = Basis::FromScale(scale);
	return result;
}

Affine Affine::FromRotation(const Quaternion& rotation) {
	Affine result;

	result.basis = Basis::FromRotation(rotation);
	return result;
}

Affine Affine::FromTranslate(const Vector3& translate) {
	Affine result;

	result.origin = translate;
	return result;
}

Affine Affine::FromMatrix(const Matrix4x4& matrix) {
	Affine result;
	result.basis[0] = Vector3{ matrix[0][0], matrix[0][1], matrix[0][2] };
	result.basis[1] = Vector3{ matrix[1][0], matrix[1][1], matrix[1][2] };
	result.basis[2] = Vector3{ matrix[2][0], matrix[2][1], matrix[2][2] };
	result.origin = Vector3{ matrix[3][0], matrix[3][1], matrix[3][2] };

	return result;
}

void Affine::invert_fast() {
	basis.transpose();
	origin = -origin * basis;
}

Affine Affine::inverse_fast() const {
	Affine result = *this;
	result.invert_fast();
	return result;
}

void Affine::invert() {
	basis.invert();
	origin = -origin * basis;
}

Affine Affine::inverse() const {
	Affine result = *this;
	result.invert();
	return result;
}

Matrix4x4 Affine::to_matrix() const {
	Matrix4x4 result;
	for (i32 i = 0; i < 3; ++i) {
		std::memcpy(result[i].data(), &basis[i], sizeof(r32) * 3);
	}
	std::memcpy(result[3].data(), &origin, sizeof(r32) * 3);
	result[3][3] = 1;
	return result;
}
