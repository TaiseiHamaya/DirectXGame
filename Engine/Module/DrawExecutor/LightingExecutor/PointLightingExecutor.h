#pragma once

#include "../BaseDrawExecutor.h"

#include "Engine/Assets/PrimitiveGeometry/PrimitiveGeometryAsset.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/StructuredBuffer/StructuredBuffer.h"
#include "Engine/Module/World/Light/PointLight/PointLightInstance.h"

#include <Library/Math/Matrix4x4.h>

class PointLightingExecutor final : public BaseDrawExecutor<PointLightInstance> {
public:
	PointLightingExecutor() = default;
	~PointLightingExecutor() = default;

	PointLightingExecutor(std::shared_ptr<const PrimitiveGeometryAsset> asset_, u32 maxInstance);
	
	__CLASS_NON_COPYABLE(PointLightingExecutor)

public:
	void reinitialize(std::shared_ptr<const PrimitiveGeometryAsset> asset_, u32 maxInstance);
	void draw_command() const override;
	void write_to_buffer(Reference<const PointLightInstance> instance) override;

private:
	std::shared_ptr<const PrimitiveGeometryAsset> asset;

	StructuredBuffer<Matrix4x4> matrices;
	StructuredBuffer<PointLightData> lightData;
};
