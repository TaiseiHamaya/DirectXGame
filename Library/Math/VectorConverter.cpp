#include "VectorConverter.h"

#include "Vector2.h"
#include "Vector3.h"

Vector2 Converter::ToVector2(const Vector3& vector) {
	return { vector.x,vector.y };
}

Vector3 Converter::ToVector3(const Vector2& vector, r32 z) {
	return { vector.x,vector.y, z };
}
