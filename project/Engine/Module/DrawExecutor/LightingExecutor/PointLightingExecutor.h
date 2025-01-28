#pragma once

#include "../BaseDrawExecutor.h"

#include "Engine/Rendering/DirectX/DirectXResourceObject/StructuredBuffer/StructuredBuffer.h"
#include "Engine/Resources/PrimitiveGeometry/PrimitiveGeometryResource.h"

#include <Library/Math/Matrix4x4.h>

struct PointLightData;

class PointLightingExecutor final : BaseDrawExecutor<PrimitiveGeometryResource> {
public:
	PointLightingExecutor() = default;
	virtual ~PointLightingExecutor() = default;

	__DRAW_EXECUTOR_CLASS(PointLightingExecutor)

public:
	void reinitialize(const std::string& primitiveGeometryName, uint32_t maxInstance) override;
	void draw_command(uint32_t InstanceCount) const override;
	void write_to_buffer(uint32_t index, const Matrix4x4& worldMatrix, const PointLightData& lightData_);

private:
	StructuredBuffer<Matrix4x4> matrices;
	StructuredBuffer<PointLightData> lightData;
};
