#pragma once

#include <cstdint>

#include <Library/Math/Affine.h>
#include <Library/Math/Color3.h>
#include <Library/Math/Matrix3x3.h>
#include <Library/Math/Matrix4x4.h>
#include <Library/Math/Vector2.h>
#include <Library/Math/Vector3.h>

enum class LighingType : uint32_t {
	None,
	Lambert,
	HalfLambert,
};

/// <summary>
/// VertexBuffer
/// </summary>
struct VertexBufferData {
	Vector3 position = CVector3::ZERO;
	Vector2 texcoord = CVector2::ZERO;
	Vector3 normal = CVector3::ZERO;
};

/// <summary>
/// InstanceTransformMatrix
/// </summary>
struct TransformMatrixData {
	Affine world;
	Basis itWorld;
};

/// <summary>
/// Material用
/// </summary>
struct MaterialBufferData {
	Color3 color;
	LighingType lighting{ LighingType::HalfLambert };
	float shininess = 50;
	uint32_t textureIndex{ 0 };
	//std::array<uint32_t, 2> padding;
	Matrix3x3 uvTransformMatrix = CMatrix3x3::IDENTITY;
};

/// <summary>
/// MatrixPaletteWell
/// </summary>
struct SkeletonMatrixPaletteWell {
	Matrix4x4 skeletonSpaceMatrix;
	Basis skeletonSpaceIT;
};
