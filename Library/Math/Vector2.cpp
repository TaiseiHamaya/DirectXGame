#include "Vector2.h"

#include <algorithm>
#include <cmath>

#include "Engine/Application/Output.h"

r32 Vector2::length() const noexcept {
	return std::sqrt(Dot(*this, *this));
}

const Vector2 Vector2::normalize() const noexcept(false) {
	WarningIf(length() == 0, "Vector2::normalize was called. But length is 0.");
	return *this / length();
}

const Vector2 Vector2::normalize_safe(const Vector2& disapproval, r32 tolerance) const noexcept {
	WarningIf(tolerance < 0, "Second argument tolerance less 0.");
	r32 length_ = length();
	if (length_ <= tolerance) {
		return disapproval;
	}
	else {
		return *this / length();
	}
}

r32 Vector2::Length(const Vector2& vector) noexcept {
	return vector.length();
}

r32 Vector2::Length(const Vector2& vector1, const Vector2& vector2) noexcept {
	return Vector2::Length(vector1 - vector2);
}

const Vector2 Vector2::Normalize(const Vector2& vector) noexcept(false) {
	return vector.normalize();
}

const Vector2 Vector2::Normalize(const Vector2& vectorFrom, const Vector2& vectorTo) {
	return Vector2::Normalize(vectorTo - vectorFrom);
}

const Vector2 Vector2::Rotate(const Vector2& vector, const r32& radian) noexcept {
	return Rotate(vector, std::sin(radian), std::cos(radian));
}

const Vector2 Vector2::Abs(const Vector2& vector) noexcept {
	return { std::abs(vector.x), std::abs(vector.y) };
}

Vector2 Vector2::Projection(const Vector2& vector, const Vector2& onto) {
	return onto * Vector2::Dot(onto, vector);
}

Vector2 Vector2::Reflect(const Vector2& input, const Vector2& normal) {
	return input - Projection(input, normal) * 2;
}

Vector2 Vector2::Clamp(const Vector2& vector, const Vector2& min, const Vector2& max) {
	return { std::clamp(vector.x, min.x, max.x),std::clamp(vector.y, min.y, max.y) };
}

Vector2 Vector2::Slerp(const Vector2& from, const Vector2& to, const r32& t) {
	r32 dot = Vector2::Dot(from, to);
	if (dot >= 0.9999f) {
		return Lerp(from, to, t).normalize();
	}

	r32 theta = std::acos(dot);
	r32 sinT = std::sin(theta);

	r32 factor0 = std::sin((1 - t) * theta) / sinT;
	r32 factor1 = std::sin(t * theta) / sinT;

	return from * factor0 + to * factor1;
}
