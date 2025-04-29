#pragma once

class Vector2;
class Vector3;

namespace Converter {

Vector2 ToVector2(const Vector3& vector);

Vector3 ToVector3(const Vector2& vector, r32 z);

};
