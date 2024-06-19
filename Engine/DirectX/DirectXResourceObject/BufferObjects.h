#pragma once

#include "Engine/Math/Color.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"

struct VertexData {
	struct Vector4 {
		Vector3 position;
		float w;
	} vertex;
	Vector2 texcoord;
	Vector3 normal;
};

struct MaterialData {
	Color color;
	int enableLighting;
	std::array<std::int32_t, 3> padding;
	Matrix4x4 uvTransform;
};

struct TransformationMatrixData {
	Matrix4x4 wvp;
	Matrix4x4 world;
};