#pragma once

#include "Library/Math/Color.h"
#include "Library/Math/Matrix4x4.h"
#include "Library/Math/Vector2.h"
#include "Library/Math/Vector3.h"

struct VertexData {
	struct Vector4 {
		Vector3 position = CVector3::ZERO;
		float w = 0.0f;
	} vertex;
	Vector2 texcoord = CVector2::ZERO;
	Vector3 normal = CVector3::ZERO;
};

struct MaterialData {
	Color color;
	std::uint32_t lighting;
	std::array<std::int32_t, 3> padding;
	Matrix4x4 uvTransform = CMatrix4x4::IDENTITY;
};
