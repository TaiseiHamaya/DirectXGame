#include "Vector3.h"

#include <cassert>
#include <cmath>
#include <algorithm>

float Vector3::length() const noexcept {
	return std::sqrt(x * x + y * y + z * z);
}

const Vector3 Vector3::normalize() const noexcept(false) {
	assert(length() != 0);
	float m = 1.0f / length();
	return *this * m;
}

const Vector3 Vector3::normalize_safe(float tolerance, const Vector3& disapproval) const noexcept {
	assert(tolerance >= 0 && disapproval.length() == 1);
	float length_ = length();
	if (length_ <= tolerance) {
		return disapproval;
	}
	else {
		float m = 1.0f / length_;
		return *this * m;
	}
}

float Vector3::Length(const Vector3& vector) noexcept {
	return vector.length();
}

float Vector3::Length(const Vector3& vector1, const Vector3& vector2) noexcept {
	return Vector3::Length(vector1 - vector2);
}

const Vector3 Vector3::Normalize(const Vector3& vector) {
	return vector.normalize();
}

const Vector3 Vector3::Normalize(const Vector3& vectorFrom, const Vector3& vectorTo) {
	return Vector3::Normalize(vectorTo - vectorFrom);
}

const Vector3 Vector3::Abs(const Vector3& vector) noexcept {
	return { std::abs(vector.x), std::abs(vector.y), std::abs(vector.z) };
}

Vector3 Vector3::Projection(const Vector3& vector, const Vector3& onto) {
	assert(onto.length() == 1);
	return onto * Vector3::DotProduct(onto, vector);
}

Vector3 Vector3::Reflect(const Vector3& input, const Vector3& normal) {
	return input - Projection(input, normal) * 2;
}

Vector3 Vector3::Clamp(const Vector3& vector, const Vector3& min, const Vector3& max) {
	return { std::clamp(vector.x, min.x, max.x),std::clamp(vector.y, min.y, max.y) ,std::clamp(vector.z, min.z, max.z) };
}

Vector3 Vector3::Slerp(const Vector3& from, const Vector3& to, const float& t) {
	float dot = Vector3::DotProduct(from, to);
	if (dot >= 0.9999f) {
		return Lerp(from, to, t).normalize();
	}

	float theta = std::acos(dot);
	float sinT = std::sin(theta);

	float factor0 = std::sin((1 - t) * theta) / sinT;
	float factor1 = std::sin(t * theta) / sinT;

	return from * factor0 + to * factor1;
}
