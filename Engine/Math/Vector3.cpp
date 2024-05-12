#include "Vector3.h"

#include <cassert>
#include <cmath>

float Vector3::length() const {
	return std::sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::normalize() const {
	assert(this->length() != 0);
	float m = 1.0f / this->length();
	return *this * m;
}

float Vector3::DotProduct(const Vector3& input1, const Vector3& input2) {
	return input1.x * input2.x + input1.y * input2.y + input1.z * input2.z;
}

//float Vector3::CrossProduct(const Vector3& input1, const Vector3& input2) {
//	return input1.x * input2.y - input1.y * input2.x;
//}

Vector3 Vector3::CrossProduct(const Vector3& vector1, const Vector3& vector2) {
	return Vector3{
		vector1.y * vector2.z - vector1.z * vector2.y,
		vector1.z * vector2.x - vector1.x * vector2.z,
		vector1.x * vector2.y - vector1.y * vector2.x,
	};
}

float Vector3::Length(const Vector3& vector) {
	return std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

float Vector3::Length(const Vector3& vector1, const Vector3& vector2) {
	return Vector3::Length(vector1 - vector2);
}

Vector3 Vector3::Normalize(const Vector3& vector) {
	assert(Vector3::Length(vector) != 0);
	float m = 1.0f / vector.length();
	return vector * m;
}

Vector3 Vector3::Normalize(const Vector3& vectorFrom, const Vector3& vectorTo) {
	assert(Vector3::Length(vectorFrom, vectorTo) != 0);
	float m = 1.0f / Vector3::Length(vectorFrom, vectorTo);
	return Vector3::Multiply(vectorTo - vectorFrom, m);
}

Vector3 Vector3::Add(const Vector3& vector1, const Vector3& vector2) {
	return vector1 + vector2;
}

Vector3 Vector3::Subtract(const Vector3& vector1, const Vector3& vector2) {
	return vector1 - vector2;
}

Vector3 Vector3::Multiply(const Vector3& vector, const float& times) {
	return vector * times;
}

Vector3 Vector3::Multiply(const Vector3& vector, const Vector3& times) {
	return Vector3{ vector.x * times.x, vector.y * times.y, vector.z * times.z };
}

Vector3 Vector3::Lerp(const Vector3& from, const Vector3& to, const float& t) {
	return from * (1 - t) + to * t;
}

Vector3 Vector3::Bezier(const Vector3& initial, const Vector3& control, const Vector3& terminal, const float t) {
	return Lerp(Lerp(initial, control, t), Lerp(control, terminal, t), t);
}

Vector3 Vector3::Abs(const Vector3& vector) {
	return { std::abs(vector.x), std::abs(vector.y), std::abs(vector.z) };
}
