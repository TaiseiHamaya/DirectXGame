#include "Quaternion.h"

#include <cmath>
#include "Transform3D.h"

Quaternion::Quaternion() noexcept :
	Quaternion(Vec3::kZero, 0) {
}

Quaternion::Quaternion(const Quaternion& rhs) noexcept :
	xyz(rhs.xyz),
	w(rhs.w) {
}

Quaternion::Quaternion(Quaternion&& rhs) noexcept :
	xyz(std::move(rhs.xyz)),
	w(std::move(rhs.w)) {
}

Quaternion::Quaternion(const Vector3& axis, float angleAxis) :
	xyz((axis != Vec3::kZero ? axis.normalize() : Vec3::kZero) * std::sin(angleAxis / 2)),
	w(std::cos(angleAxis / 2)) {
}

Quaternion::Quaternion(float x, float y, float z, float w) :
	xyz(Vector3{ x,y,z }),
	w(w) {
}

Quaternion::Quaternion(float pitch, float yaw, float roll) {
	float cos_pitch = std::cos(pitch / 2);
	float cos_yaw = std::cos(yaw / 2);
	float cos_roll = std::cos(roll / 2);
	float sin_pitch = std::sin(pitch / 2);
	float sin_yaw = std::sin(yaw / 2);
	float sin_roll = std::sin(roll / 2);
	xyz = {
		sin_pitch * cos_yaw * cos_roll + cos_pitch * sin_yaw * sin_roll,
		cos_pitch * sin_yaw * cos_roll + sin_pitch * cos_yaw * sin_roll,
		cos_pitch * cos_yaw * sin_roll + sin_pitch * sin_yaw * cos_roll,
	};
	w = cos_pitch * cos_yaw * cos_roll + sin_pitch * sin_yaw * sin_roll;
}

Quaternion& Quaternion::operator=(const Quaternion& rhs) noexcept {
	xyz = rhs.xyz;
	w = rhs.w;
	return *this;
}

Quaternion& Quaternion::operator=(Quaternion&& rhs) noexcept {
	xyz = std::move(rhs.xyz);
	w = std::move(rhs.w);
	return *this;
}

bool Quaternion::operator==(const Quaternion& rhs) const noexcept {
	return xyz == rhs.xyz && w == rhs.w;
}

bool Quaternion::operator!=(const Quaternion& rhs) const noexcept {
	return !(*this == rhs);
}

Quaternion Quaternion::operator*(const Quaternion& rhs) const {
	Vector3 result = rhs.xyz * w + xyz * rhs.w + Vector3::CrossProduct(rhs.xyz, xyz);
	return Quaternion{
		result.x,result.y,result.z,
		w * rhs.w - Vector3::DotProduct(xyz, rhs.xyz) };
}

Quaternion& Quaternion::operator*=(const Quaternion& rhs) {
	*this = *this * rhs;
	return *this;
}

const Matrix4x4 Quaternion::to_matrix() const {
	float xx = xyz.x * xyz.x;
	float xy = xyz.x * xyz.y;
	float xz = xyz.x * xyz.z;
	float xw = xyz.x * w;
	float yy = xyz.y * xyz.y;
	float yz = xyz.y * xyz.z;
	float yw = xyz.y * w;
	float zz = xyz.z * xyz.z;
	float zw = xyz.z * w;

	return {
		{{1 - 2 * (yy + zz), 2 * (xy - zw), 2 * (xz + yw), 0},
		{2 * (xy + zw), 1 - 2 * (xx + zz), 2 * (yz - xw), 0},
		{2 * (xz - yw), 2 * (yz + xw), 1 - 2 * (xx + yy), 0},
		{0,0,0,1}}
	};
}