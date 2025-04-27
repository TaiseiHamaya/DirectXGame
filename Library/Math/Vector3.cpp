#include "Vector3.h"

#include <cassert>
#include <cmath>
#include <algorithm>

#include "Engine/Application/Output.h"

r32 Vector3::length() const noexcept {
	return std::sqrt(Dot(*this, *this));
}

Vector3 Vector3::normalize() const noexcept(false) {
	WarningIf(length() == 0, "Vector3::normalize was called. But length is 0.");
	return *this / length();;
}

Vector3 Vector3::normalize_safe(r32 tolerance, const Vector3& disapproval) const noexcept {
	WarningIf(tolerance < 0, "First argument tolerance less 0.");
	r32 length_ = length();
	if (length_ <= tolerance) {
		return disapproval;
	}
	else {
		return *this / length_;
	}
}

r32 Vector3::Length(const Vector3& vector) noexcept {
	return vector.length();
}

r32 Vector3::Length(const Vector3& vector1, const Vector3& vector2) noexcept {
	return Vector3::Length(vector1 - vector2);
}

Vector3 Vector3::Normalize(const Vector3& vector) {
	return vector.normalize();
}

Vector3 Vector3::Normalize(const Vector3& vectorFrom, const Vector3& vectorTo) {
	return Vector3::Normalize(vectorTo - vectorFrom);
}

Vector3 Vector3::Abs(const Vector3& vector) noexcept {
	return { std::abs(vector.x), std::abs(vector.y), std::abs(vector.z) };
}

Vector3 Vector3::Projection(const Vector3& vector, const Vector3& onto) {
	return onto * Vector3::Dot(onto, vector);
}

Vector3 Vector3::Reflect(const Vector3& input, const Vector3& normal) {
	return input - Projection(input, normal) * 2;
}

Vector3 Vector3::Clamp(const Vector3& vector, const Vector3& min, const Vector3& max) {
	return { std::clamp(vector.x, min.x, max.x),std::clamp(vector.y, min.y, max.y) ,std::clamp(vector.z, min.z, max.z) };
}

Vector3 Vector3::Slerp(const Vector3& from, const Vector3& to, const r32& t) {
	r32 dot = Vector3::Dot(from, to);
	if (dot >= 0.9999f) {
		return Lerp(from, to, t).normalize();
	}

	r32 theta = std::acos(dot);
	r32 sinT = std::sin(theta);

	r32 factor0 = std::sin((1 - t) * theta) / sinT;
	r32 factor1 = std::sin(t * theta) / sinT;

	return from * factor0 + to * factor1;
}
