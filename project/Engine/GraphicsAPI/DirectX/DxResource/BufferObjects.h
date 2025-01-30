#pragma once

#include <Library/Math/Color3.h>
#include <Library/Math/Matrix4x4.h>
#include <Library/Math/Vector2.h>
#include <Library/Math/Vector3.h>

struct VertexBufferData {
	struct Vector4 {
		Vector3 position = CVector3::ZERO;
		float w = 0.0f;
	} vertex;
	Vector2 texcoord = CVector2::ZERO;
	Vector3 normal = CVector3::ZERO;
};

struct MaterialBufferData {
	Color3 color;
	std::uint32_t lighting;
	float shininess = 50;
	std::array<uint32_t, 3> padding;
	Matrix4x4 uvTransformMatrix = CMatrix4x4::IDENTITY;
};
