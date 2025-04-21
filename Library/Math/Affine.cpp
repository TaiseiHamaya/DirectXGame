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
	for (int i = 0; i < 3; ++i) {
		std::memcpy(result[i].data(), &basis[i], sizeof(float) * 3);
	}
	std::memcpy(result[3].data(), &origin, sizeof(float) * 3);
	result[3][3] = 1;
	return result;
}
