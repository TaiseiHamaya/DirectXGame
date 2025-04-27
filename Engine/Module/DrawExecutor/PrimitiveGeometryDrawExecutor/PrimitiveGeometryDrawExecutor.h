#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "../BaseDrawExecutor.h"

#include "Engine/Assets/PrimitiveGeometry/PrimitiveGeometryAsset.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/StructuredBuffer/StructuredBuffer.h"

#include <Library/Math/Matrix4x4.h>

class PrimitiveGeometryDrawExecutor final : public BaseDrawExecutor<Matrix4x4> {
public:
	PrimitiveGeometryDrawExecutor() = default;
	~PrimitiveGeometryDrawExecutor() = default;

	PrimitiveGeometryDrawExecutor(std::shared_ptr<const PrimitiveGeometryAsset> asset_, u32 maxInstance);

	__CLASS_NON_COPYABLE(PrimitiveGeometryDrawExecutor)

public:
	void reinitialize(std::shared_ptr<const PrimitiveGeometryAsset> asset_, u32 maxInstance);
	void draw_command() const override;
	void write_to_buffer(Reference<const Matrix4x4> worldMatrix) override;

private:
	std::shared_ptr<const PrimitiveGeometryAsset> asset;
	StructuredBuffer<Matrix4x4> matrices;
};

#endif // _DEBUG
