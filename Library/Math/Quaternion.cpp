#include "Quaternion.h"

#include <cmath>

#include "Basis.h"
#include "Definition.h"
#include "Matrix4x4.h"

Quaternion::Quaternion() noexcept : Quaternion{ 0,0,0,1 } {}

Quaternion::Quaternion(const Vector3& xyz_, r32 w_) noexcept :
	xyz(xyz_),
	w(w_) {
}

Quaternion::Quaternion(r32 x, r32 y, r32 z, r32 w) noexcept :
	xyz(Vector3{ x,y,z }),
	w(w) {
}

Quaternion Quaternion::AngleAxis(const Vector3& axis, r32 angleAxis) {
	Quaternion result;
	result.xyz = axis.normalize_safe() * std::sin(angleAxis / 2);
	result.w = std::cos(angleAxis / 2);
	return result;
}

Quaternion Quaternion::EulerRadian(r32 pitch, r32 yaw, r32 roll) noexcept {
	r32 cosPitch = std::cos(pitch / 2);
	r32 cos_yaw = std::cos(yaw / 2);
	r32 cos_roll = std::cos(roll / 2);
	r32 sin_pitch = std::sin(pitch / 2);
	r32 sin_yaw = std::sin(yaw / 2);
	r32 sin_roll = std::sin(roll / 2);
	Quaternion result;
	result.xyz = {
		sin_pitch * cos_yaw * cos_roll - cosPitch * sin_yaw * sin_roll,
		cosPitch * sin_yaw * cos_roll + sin_pitch * cos_yaw * sin_roll,
		cosPitch * cos_yaw * sin_roll - sin_pitch * sin_yaw * cos_roll,
	};
	result.w = cosPitch * cos_yaw * cos_roll + sin_pitch * sin_yaw * sin_roll;
	return result;
}

Quaternion Quaternion::EulerRadian(const Vector3& rotate) noexcept {
	// 分解して定義
	return EulerRadian(rotate.x, rotate.y, rotate.z);
}

Quaternion Quaternion::EulerDegree(const Vector3& rotate) noexcept {
	// ラジアン変換して定義
	return EulerRadian(rotate.x * ToRadian, rotate.y * ToRadian, rotate.z * ToRadian);
}

Quaternion Quaternion::EulerDegree(r32 pitch, r32 yaw, r32 roll) noexcept {
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
	Vector3 resultV = rhs.xyz * w + xyz * rhs.w + Vector3::Cross(xyz, rhs.xyz);
	return Quaternion{
		resultV, w * rhs.w - Vector3::Dot(xyz, rhs.xyz)
	};
}

Quaternion& Quaternion::operator*=(const Quaternion& rhs) noexcept {
	*this = rhs * *this;
	return *this;
}

Quaternion Quaternion::operator*(r32 times) const noexcept {
	return { xyz * times, w * times };
}

Quaternion& Quaternion::operator*=(r32 times) noexcept {
	*this = *this * times;
	return *this;
}

Matrix4x4 Quaternion::to_matrix() const noexcept {
	r32 xx = xyz.x * xyz.x;
	r32 xy = xyz.x * xyz.y;
	r32 xz = xyz.x * xyz.z;
	r32 xw = xyz.x * w;
	r32 yy = xyz.y * xyz.y;
	r32 yz = xyz.y * xyz.z;
	r32 yw = xyz.y * w;
	r32 zz = xyz.z * xyz.z;
	r32 zw = xyz.z * w;
	r32 ww = w * w;

	return {
		{{ww + xx - yy - zz, 2 * (xy + zw), 2 * (xz - yw), 0},
		{2 * (xy - zw),ww - xx + yy - zz, 2 * (yz + xw), 0},
		{2 * (xz + yw), 2 * (yz - xw), ww - xx - yy + zz , 0},
		{0,0,0,1}}
	};
}

Basis Quaternion::to_basis() const noexcept {
	r32 xx = xyz.x * xyz.x;
	r32 xy = xyz.x * xyz.y;
	r32 xz = xyz.x * xyz.z;
	r32 xw = xyz.x * w;
	r32 yy = xyz.y * xyz.y;
	r32 yz = xyz.y * xyz.z;
	r32 yw = xyz.y * w;
	r32 zz = xyz.z * xyz.z;
	r32 zw = xyz.z * w;
	r32 ww = w * w;

	return {
		{ww + xx - yy - zz, 2 * (xy + zw), 2 * (xz - yw)},
		{2 * (xy - zw),ww - xx + yy - zz, 2 * (yz + xw)},
		{2 * (xz + yw), 2 * (yz - xw), ww - xx - yy + zz}
	};
}

r32 Quaternion::length() const noexcept {
	Vector3 v2 = Vector3::Multiply(xyz, xyz);
	return std::sqrt(v2.x + v2.y + v2.z + w * w);
}

Quaternion Quaternion::inverse() const noexcept {
	return { -xyz.x, -xyz.y, -xyz.z, w };
}

Quaternion Quaternion::normalize() const noexcept {
	return *this * (1 / length());
}

const Vector3& Quaternion::vector() const noexcept {
	return xyz;
}

const r32& Quaternion::real() const noexcept {
	return w;
}

Quaternion Quaternion::FromToRotation(const Vector3& from, const Vector3& to) {
	r32 cos = Vector3::Dot(from, to);
	constexpr r32 PERMISSIBLE = 1e-6f;
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
		Vector3 axis = Vector3::Cross(from, orthogonal).normalize();
		return Quaternion{ axis, 0 };
	}

	Vector3 axis = Vector3::Cross(from, to);

	r32 angle = std::acos(cos);

	//r32 halfcos = std::sqrt((1 - cos) / 2);
	//r32 halfsin = std::sqrt((1 + cos) / 2);

	//Quaternion result;
	//result.xyz = axis.normalize() * halfsin;
	//result.w = halfcos;
	//return result;

	return Quaternion::AngleAxis(axis, angle);
}

Quaternion Quaternion::LookForward(const Vector3& forward, const Vector3& upward) {
	Quaternion lookRotation = FromToRotation(CVector3::BASIS_Z, forward);
	Vector3 xAxisHorizontal = Vector3::Cross(upward, forward).normalize_safe();
	Vector3 yAxisAfterRotate = Vector3::Cross(forward, xAxisHorizontal);

	Vector3 yAxisBeforeModify = CVector3::BASIS_Y * lookRotation;
	Quaternion modifyRotation = FromToRotation(yAxisBeforeModify, yAxisAfterRotate);
	return modifyRotation * lookRotation;
}

Quaternion Quaternion::Slerp(const Quaternion& internal, const Quaternion& terminal, r32 t) noexcept {
	r32 dot = Vector3::Dot(internal.xyz, terminal.xyz) + internal.w * terminal.w;
	Quaternion internal_;
	if (dot < 0) {
		dot *= -1;
		internal_ = internal * -1;
	}
	else {
		internal_ = internal;
	}

	r32 theta = std::acos(dot);

	r32 sint = std::sin(theta);
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

Quaternion Quaternion::SlerpFar(const Quaternion& internal, const Quaternion& terminal, r32 t) noexcept {
	r32 dot = Vector3::Dot(internal.xyz, terminal.xyz) + internal.w * terminal.w;
	Quaternion internal_;
	if (dot >= 0) {
		dot *= -1;
		internal_ = internal * -1;
	}
	else {
		internal_ = internal;
	}

	r32 theta = std::acos(dot);

	r32 sint = std::sin(theta);
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

Quaternion Quaternion::SlerpClockwise(const Quaternion& internal, const Quaternion& terminal, r32 t, const Vector3& axis) noexcept {
	Vector3 internalV = CVector3::FORWARD * internal;
	Vector3 terminalV = CVector3::FORWARD * terminal;
	Vector3 cross = Vector3::Cross(internalV, terminalV);

	Quaternion internal_;
	if (std::signbit(Vector3::Dot(cross, axis)) != std::signbit(internal.w)) {
		internal_ = internal * -1;
	}
	else {
		internal_ = internal;
	}

	r32 dot = Vector3::Dot(internal_.xyz, terminal.xyz) + internal_.w * terminal.w;

	r32 theta = std::acos(dot);

	r32 sint = std::sin(theta);
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

Vector3 operator*(const Vector3& vector, const Quaternion& quaternion) {
	Quaternion vectorQuaternion = Quaternion{ vector, 0.0f };
	return (quaternion * vectorQuaternion * quaternion.inverse()).xyz;
}
