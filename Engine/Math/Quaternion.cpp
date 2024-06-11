#include "Quaternion.h"

#include <cmath>

Quaternion::Quaternion() noexcept :
	Quaternion(CVector3::ZERO, 0) {
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
	xyz((axis != CVector3::ZERO ? axis.normalize() : CVector3::ZERO)* std::sin(angleAxis / 2)),
	w(std::cos(angleAxis / 2)) {
}

Quaternion::Quaternion(float x, float y, float z, float w) noexcept :
	xyz(Vector3{ x,y,z }),
	w(w) {
}

Quaternion::Quaternion(float pitch, float yaw, float roll) noexcept {
	float cos_pitch = std::cos(pitch / 2);
	float cos_yaw = std::cos(yaw / 2);
	float cos_roll = std::cos(roll / 2);
	float sin_pitch = std::sin(pitch / 2);
	float sin_yaw = std::sin(yaw / 2);
	float sin_roll = std::sin(roll / 2);
	xyz = {
		sin_pitch * cos_yaw * cos_roll - cos_pitch * sin_yaw * sin_roll,
		cos_pitch * sin_yaw * cos_roll + sin_pitch * cos_yaw * sin_roll,
		cos_pitch * cos_yaw * sin_roll - sin_pitch * sin_yaw * cos_roll,
	};
	w = cos_pitch * cos_yaw * cos_roll + sin_pitch * sin_yaw * sin_roll;
}

Quaternion::Quaternion(const Vector3& rotate) noexcept : Quaternion{ rotate.x, rotate.y, rotate.z } {
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

Quaternion Quaternion::operator*(const Quaternion& rhs) const noexcept {
	Vector3 resultV = rhs.xyz * w + xyz * rhs.w + Vector3::CrossProduct(rhs.xyz, xyz);
	return Quaternion{
		resultV.x,resultV.y,resultV.z,
		w * rhs.w - Vector3::DotProduct(xyz, rhs.xyz) };
}

Quaternion& Quaternion::operator*=(const Quaternion& rhs) noexcept {
	*this = *this * rhs;
	return *this;
}

Quaternion Quaternion::operator*(float times) const noexcept {
	return { xyz.x * times, xyz.y * times, xyz.z * times, w * times };
}

Quaternion& Quaternion::operator*=(float times) noexcept {
	*this = *this * times;
	return *this;
}

const Matrix4x4 Quaternion::to_matrix() const noexcept {
	float xx = xyz.x * xyz.x;
	float xy = xyz.x * xyz.y;
	float xz = xyz.x * xyz.z;
	float xw = xyz.x * w;
	float yy = xyz.y * xyz.y;
	float yz = xyz.y * xyz.z;
	float yw = xyz.y * w;
	float zz = xyz.z * xyz.z;
	float zw = xyz.z * w;
	float ww = w * w;

	return {
		{{ww + xx - yy - zz, 2 * (xy + zw), 2 * (xz - yw), 0},
		{2 * (xy - zw),ww - xx + yy - zz, 2 * (yz + xw), 0},
		{2 * (xz + yw), 2 * (yz - xw), ww - xx - yy + zz , 0},
		{0,0,0,1}}
	};
}

const float Quaternion::length() const noexcept {
	Vector3 v2 = Vector3::Multiply(xyz, xyz);
	return std::sqrt(v2.x + v2.y + v2.z + w * w);
}

const Quaternion Quaternion::inverse() const noexcept {
	return { -xyz.x, -xyz.y, -xyz.z, w };
}

const Quaternion Quaternion::Slerp(const Quaternion& internal, const Quaternion& terminal, float t) noexcept {
	float dot = Vector3::DotProduct(internal.xyz, terminal.xyz) + internal.w * terminal.w;
	Quaternion internal_;
	if (dot < 0) {
		dot *= -1;
		internal_ = internal.inverse();
	}
	else {
		internal_ = internal;
	}

	float theta = std::acos(dot);

	float sint = std::sin(theta);
	Quaternion rResult;
	Quaternion lResult;
	if (dot >= 1.0f - 0.005f) {
		rResult = internal_ * (1.0f - t);
		lResult = terminal * t;
	}
	else {
		rResult = internal_ * (std::sin((1.0f - t) * theta) / sint);
		lResult = terminal * (std::sin(t * theta) / sint);
	}
	return { rResult.xyz.x + lResult.xyz.x, rResult.xyz.y + lResult.xyz.y, rResult.xyz.z + lResult.xyz.z, rResult.w + lResult.w };
}

const Quaternion& Quaternion::Identity() noexcept {
	static Quaternion identity{ 0,0,0,1 };
	return identity;
}
