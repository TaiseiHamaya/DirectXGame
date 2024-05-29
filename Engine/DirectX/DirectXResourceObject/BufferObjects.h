#pragma once

#include "Engine/Math/Color.h"
#include "Engine/Math/Matrix.h"
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
	int32_t padding[3];
	Matrix4x4 uvTransform;
};

struct TransformationMatrixData {
	Matrix4x4 wvp;
	Matrix4x4 world;
};