#include "Vector2.h"

#include <cmath>
#include <algorithm>

#include "Engine/Application/Output.h"

float Vector2::length() const noexcept {
	return std::sqrt(DotProduct(*this, *this));
}

const Vector2 Vector2::normalize() const noexcept(false) {
	WarningIf(length() == 0, "Vector2::normalize was called. But length is 0.");
	return *this / length();
}

const Vector2 Vector2::normalize_safe(float tolerance, const Vector2& disapproval) const noexcept {
	WarningIf(tolerance < 0, "First argument tolerance less 0.");
	float length_ = length();
	if (length_ <= tolerance) {
		return disapproval;
	}
	else {
		return *this / length();
	}
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

Vector2 Vector2::Projection(const Vector2& vector, const Vector2& onto) {
	return onto * Vector2::DotProduct(onto, vector);
}

Vector2 Vector2::Reflect(const Vector2& input, const Vector2& normal) {
	return input - Projection(input, normal) * 2;
}

Vector2 Vector2::Clamp(const Vector2& vector, const Vector2& min, const Vector2& max) {
	return { std::clamp(vector.x, min.x, max.x),std::clamp(vector.y, min.y, max.y) };
}

Vector2 Vector2::Slerp(const Vector2& from, const Vector2& to, const float& t) {
	float dot = Vector2::DotProduct(from, to);
	if (dot >= 0.9999f) {
		return Lerp(from, to, t).normalize();
	}

	float theta = std::acos(dot);
	float sinT = std::sin(theta);

	float factor0 = std::sin((1 - t) * theta) / sinT;
	float factor1 = std::sin(t * theta) / sinT;

	return from * factor0 + to * factor1;
}
