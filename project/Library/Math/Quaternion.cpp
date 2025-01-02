#include "Quaternion.h"

#include <cmath>

#include "Definition.h"
#include "Basis.h"

Quaternion::Quaternion() noexcept : Quaternion{ 0,0,0,1 } {}

Quaternion::Quaternion(const Vector3& xyz_, float w_) noexcept :
	xyz(xyz_),
	w(w_) {
}

Quaternion::Quaternion(float x, float y, float z, float w) noexcept :
	xyz(Vector3{ x,y,z }),
	w(w) {
}

const Quaternion Quaternion::AngleAxis(const Vector3& axis, float angleAxis) {
	Quaternion result;
	result.xyz = axis.normalize_safe() * std::sin(angleAxis / 2);
	result.w = std::cos(angleAxis / 2);
	return result;
}

const Quaternion Quaternion::EulerRadian(float pitch, float yaw, float roll) noexcept {
	float cosPitch = std::cos(pitch / 2);
	float cos_yaw = std::cos(yaw / 2);
	float cos_roll = std::cos(roll / 2);
	float sin_pitch = std::sin(pitch / 2);
	float sin_yaw = std::sin(yaw / 2);
	float sin_roll = std::sin(roll / 2);
	Quaternion result;
	result.xyz = {
		sin_pitch * cos_yaw * cos_roll - cosPitch * sin_yaw * sin_roll,
		cosPitch * sin_yaw * cos_roll + sin_pitch * cos_yaw * sin_roll,
		cosPitch * cos_yaw * sin_roll - sin_pitch * sin_yaw * cos_roll,
	};
	result.w = cosPitch * cos_yaw * cos_roll + sin_pitch * sin_yaw * sin_roll;
	return result;
}

const Quaternion Quaternion::EulerRadian(const Vector3& rotate) noexcept {
	// 分解して定義
	return EulerRadian(rotate.x, rotate.y, rotate.z);
}

const Quaternion Quaternion::EulerDegree(const Vector3& rotate) noexcept {
	// ラジアン変換して定義
	return EulerRadian(rotate.x * ToRadian, rotate.y * ToRadian, rotate.z * ToRadian);
}

const Quaternion Quaternion::EulerDegree(float pitch, float yaw, float roll) noexcept {
	// ラジアン変換して定義
	return EulerRadian(pitch * ToRadian, yaw * ToRadian, roll * ToRadian);
}

bool Quaternion::operator==(const Quaternion& rhs) const noexcept {
	return xyz == rhs.xyz && w == rhs.w;
}

bool Quaternion::operator!=(const Quaternion& rhs) const noexcept {
	return !(*this == rhs);
}

Quaternion Quaternion::operator*(const Quaternion& rhs) const noexcept {
	Vector3 resultV = rhs.xyz * w + xyz * rhs.w + Vector3::CrossProduct(xyz, rhs.xyz);
	return Quaternion{
		resultV, w * rhs.w - Vector3::DotProduct(xyz, rhs.xyz)
	};
}

Quaternion& Quaternion::operator*=(const Quaternion& rhs) noexcept {
	*this = rhs * *this;
	return *this;
}

Quaternion Quaternion::operator*(float times) const noexcept {
	return { xyz * times, w * times };
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

Basis Quaternion::to_basis() const noexcept {
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
		{ww + xx - yy - zz, 2 * (xy + zw), 2 * (xz - yw)},
		{2 * (xy - zw),ww - xx + yy - zz, 2 * (yz + xw)},
		{2 * (xz + yw), 2 * (yz - xw), ww - xx - yy + zz}
	};
}

const float Quaternion::length() const noexcept {
	Vector3 v2 = Vector3::Multiply(xyz, xyz);
	return std::sqrt(v2.x + v2.y + v2.z + w * w);
}

const Quaternion Quaternion::inverse() const noexcept {
	return { -xyz.x, -xyz.y, -xyz.z, w };
}

const Quaternion Quaternion::normalize() const noexcept {
	return *this * (1 / length());
}

const Vector3& Quaternion::vector() const noexcept {
	return xyz;
}

const float& Quaternion::real() const noexcept {
	return w;
}

const Quaternion Quaternion::FromToRotation(const Vector3& from, const Vector3& to) {
	float cos = Vector3::DotProduct(from, to);
	constexpr float PERMISSIBLE = 1e-6f;
	// from == toの場合
	if (cos >= 1 - PERMISSIBLE) {
		return CQuaternion::IDENTITY;
	}
	// from == -toの場合
	else if (cos < -1 + PERMISSIBLE) {
		Vector3 orthogonal = CVector3::BASIS_X;
		if (std::abs(from.x) > 1 - PERMISSIBLE) {
			orthogonal = CVector3::BASIS_Y;
		}
		Vector3 axis = Vector3::CrossProduct(from, orthogonal).normalize();
		return Quaternion{ axis, 0 };
	}

	Vector3 axis = Vector3::CrossProduct(from, to);

	float angle = std::acos(cos);

	//float halfcos = std::sqrt((1 - cos) / 2);
	//float halfsin = std::sqrt((1 + cos) / 2);

	//Quaternion result;
	//result.xyz = axis.normalize() * halfsin;
	//result.w = halfcos;
	//return result;

	return Quaternion::AngleAxis(axis, angle);
}

const Quaternion Quaternion::LookForward(const Vector3& forward, const Vector3& upward) {
	Quaternion lookRotation = FromToRotation(CVector3::BASIS_Z, forward);
	Vector3 xAxisHorizontal = Vector3::CrossProduct(upward, forward).normalize_safe();
	Vector3 yAxisAfterRotate = Vector3::CrossProduct(forward, xAxisHorizontal);

	Vector3 yAxisBeforeModify = CVector3::BASIS_Y * lookRotation;
	Quaternion modifyRotation = FromToRotation(yAxisBeforeModify, yAxisAfterRotate);
	return modifyRotation * lookRotation;
}

const Quaternion Quaternion::Slerp(const Quaternion& internal, const Quaternion& terminal, float t) noexcept {
	float dot = Vector3::DotProduct(internal.xyz, terminal.xyz) + internal.w * terminal.w;
	Quaternion internal_;
	if (dot < 0) {
		dot *= -1;
		internal_ = internal * -1;
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
	Quaternion result;
	result.xyz = rResult.xyz + lResult.xyz;
	result.w = rResult.w + lResult.w;
	return result;
}

const Vector3 operator*(const Vector3& vector, const Quaternion& quaternion) {
	Quaternion vectorQuaternion = Quaternion{ vector, 0.0f };
	return (quaternion * vectorQuaternion * quaternion.inverse()).xyz;
}
