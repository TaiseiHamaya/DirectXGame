#include "Vector2.h"

#include "Vector3.h"

#include <cassert>
#include <cmath>

float Vector2::length() const {
	return std::sqrt(x * x + y * y);
}

Vector2 Vector2::normalize() const {
	assert(this->length() != 0);
	float m = 1.0f / this->length();
	return *this * m;
}

Vector3 Vector2::convert_3d(float z) const {
	return Vector3{ x,y,z };
}

float Vector2::DotProduct(const Vector2& input1, const Vector2& input2) {
	return input1.x * input2.x + input1.y * input2.y;
}

float Vector2::CrossProduct(const Vector2& input1, const Vector2& input2) {
	return input1.x * input2.y - input1.y * input2.x;
}

float Vector2::Length(const Vector2& vector) {
	return sqrtf(vector.x * vector.x + vector.y * vector.y);
}

float Vector2::Length(const Vector2& vector1, const Vector2& vector2) {
	return Vector2::Length(vector1 - vector2);
}

Vector2 Vector2::Normalize(const Vector2& vector) {
	assert(Vector2::Length(vector) != 0);
	float m = 1.0f / Vector2::Length(vector);
	return Vector2::Multiply(vector, m);
}

Vector2 Vector2::Normalize(const Vector2& vectorFrom, const Vector2& vectorTo) {
	assert(Vector2::Length(vectorFrom, vectorTo) != 0);
	float m = 1.0f / Vector2::Length(vectorFrom, vectorTo);
	return Vector2::Multiply(vectorTo - vectorFrom, m);
}

Vector2 Vector2::Add(const Vector2& vector1, const Vector2& vector2) {
	return vector1 + vector2;
}

Vector2 Vector2::Subtract(const Vector2& vector1, const Vector2& vector2) {
	return vector1 - vector2;
}

Vector2 Vector2::Multiply(const Vector2& vector, const float& times) {
	return Vector2{ vector.x * times, vector.y * times };
}

Vector2 Vector2::Multiply(const Vector2& vector, const Vector2& times) {
	return Vector2{ vector.x * times.x, vector.y * times.y };
}

Vector2 Vector2::Lerp(const Vector2& from, const Vector2& to, const float& t) {
	return Vector2::Add(Vector2::Multiply(from, (1 - t)), Vector2::Multiply(to, t));
}

Vector2 Vector2::Rotate(const Vector2& vector, const float& radian) {
	return Vector2{
		vector.x * cosf(radian) - vector.y * sinf(radian),
		vector.x * sinf(radian) + vector.y * cosf(radian) };
}

Vector2 Vector2::Rotate(const Vector2& vector, const float sintheta, const float costheta) {
	return Vector2{
		vector.x * costheta - vector.y * sintheta,
		vector.x * sintheta + vector.y * costheta };
}

Vector2 Vector2::Bezier(const Vector2& initial, const Vector2& control, const Vector2& terminal, const float t) {
	return Lerp(Lerp(initial, control, t), Lerp(control, terminal, t), t);
}

Vector2 Vector2::Abs(const Vector2& vector) {
	return { std::abs(vector.x), std::abs(vector.y) };
}
