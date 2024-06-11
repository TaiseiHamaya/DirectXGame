#include "Vector2.h"

#include <cassert>
#include <cmath>

float Vector2::length() const noexcept {
	return std::sqrt(x * x + y * y);
}

const Vector2 Vector2::normalize() const noexcept(false) {
	assert(this->length() != 0);
	float m = 1.0f / this->length();
	return *this * m;
}

float Vector2::Length(const Vector2& vector) noexcept {
	return vector.length();
}

float Vector2::Length(const Vector2& vector1, const Vector2& vector2) noexcept {
	return Vector2::Length(vector1 - vector2);
}

const Vector2 Vector2::Normalize(const Vector2& vector) noexcept(false) {
	return vector.normalize();
}

const Vector2 Vector2::Normalize(const Vector2& vectorFrom, const Vector2& vectorTo) {
	return Vector2::Normalize(vectorTo - vectorFrom);
}

const Vector2 Vector2::Rotate(const Vector2& vector, const float& radian) noexcept {
	return Rotate(vector, std::sin(radian), std::cos(radian));
}

const Vector2 Vector2::Abs(const Vector2& vector) noexcept {
	return { std::abs(vector.x), std::abs(vector.y) };
}
