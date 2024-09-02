#pragma once

#include "Engine/Game/Color/Color.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"

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
	Matrix4x4 uvTransform;
};

struct TransformationMatrixData {
	Matrix4x4 wvp;
	Matrix4x4 world;
};