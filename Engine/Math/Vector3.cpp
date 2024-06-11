#include "Vector3.h"

#include <cassert>
#include <cmath>

float Vector3::length() const noexcept {
	return std::sqrt(x * x + y * y + z * z);
}

const Vector3 Vector3::normalize() const noexcept(false) {
	assert(length() != 0);
	float m = 1.0f / length();
	return *this * m;
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
