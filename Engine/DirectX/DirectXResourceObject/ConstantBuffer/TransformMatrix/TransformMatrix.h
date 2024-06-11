#pragma once

#include "Engine/DirectX/DirectXResourceObject/BufferObjects.h"
#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h"

class TransformMatrix final : public ConstantBuffer<TransformationMatrixData> {
public:
	TransformMatrix() noexcept(false);
	~TransformMatrix() noexcept = default;

	TransformMatrix(const TransformationMatrixData& transformMatrixData) noexcept(false);

public:
	void set_transformation_matrix_data(
		Matrix4x4&& world, 
		Matrix4x4&& wvp
	) noexcept;
};
