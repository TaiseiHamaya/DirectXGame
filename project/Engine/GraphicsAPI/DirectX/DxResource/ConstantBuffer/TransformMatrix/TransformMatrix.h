#pragma once

#include "Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h"

#include <Library/Math/Matrix4x4.h>

class Affine;

struct TransformMatrixData {
	Matrix4x4 world;
	Matrix4x4 inversed;
};

class TransformMatrix final : public ConstantBuffer<TransformMatrixData> {
public:
	TransformMatrix() noexcept(false);
	~TransformMatrix() noexcept;

public:
	void set_transformation_matrix_data(const Affine& world) noexcept;
};
