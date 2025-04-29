#pragma once

#include <Library/Math/Affine.h>
#include <Library/Math/Color3.h>
#include <Library/Math/Color4.h>
#include <Library/Math/Matrix3x3.h>
#include <Library/Math/Matrix4x4.h>
#include <Library/Math/Vector2.h>
#include <Library/Math/Vector3.h>

enum class LighingType : u32 {
	None,
	Lambert,
	HalfLambert,
};

/// <summary>
/// VertexBuffer
/// </summary>
struct VertexDataBuffer {
	Vector3 position = CVector3::ZERO;
	Vector2 texcoord = CVector2::ZERO;
	Vector3 normal = CVector3::ZERO;
};

/// <summary>
/// InstanceTransformMatrix
/// </summary>
struct TransformMatrixDataBuffer {
	Affine world;
	Basis itWorld;
};

/// <summary>
/// MaterialBuffer
/// Color3バージョン
/// </summary>
struct MaterialDataBuffer3 {
	Color3 color;
	LighingType lighting{ LighingType::HalfLambert };
	r32 shininess = 50;
	u32 textureIndex{ 0 };
	Matrix3x3 uvTransformMatrix = CMatrix3x3::IDENTITY;
};

/// <summary>
/// MaterialBuffer
/// Color4バージョン
/// </summary>
struct MaterialDataBuffer4 {
	Color4 color;
	LighingType lighting{ LighingType::HalfLambert };
	r32 shininess = 50;
	u32 textureIndex{ 0 };
	Matrix3x3 uvTransformMatrix = CMatrix3x3::IDENTITY;
};

/// <summary>
/// MatrixPaletteWell
/// </summary>
struct SkeletonMatrixPaletteWellBuffer {
	Matrix4x4 skeletonSpaceMatrix;
	Basis skeletonSpaceIT;
};
