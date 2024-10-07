#pragma once

#include "Engine/Math/Matrix4x4.h"
#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h"

class TransformMatrix final : public ConstantBuffer<Matrix4x4> {
public:
	TransformMatrix() noexcept(false);
	~TransformMatrix() noexcept;

	TransformMatrix(const Matrix4x4& transformMatrixData) noexcept(false);

public:
	void set_transformation_matrix_data(const Matrix4x4& world) noexcept;
};
