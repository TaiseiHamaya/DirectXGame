#pragma once

#include <mutex>

#include "../../BaseDrawExecutor.h"

#include <Library/Utility/Template/Reference.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/StructuredBuffer/StructuredBuffer.h"
#include "Engine/Module/World/Mesh/Primitive/IPrimitiveInstance.h"

class Rect3d;
struct RectData;

class Rect3dDrawExecutor final : public BaseDrawExecutor<Rect3d> {
public:
	Rect3dDrawExecutor() noexcept = default;
	~Rect3dDrawExecutor() noexcept = default;

	__CLASS_NON_COPYABLE(Rect3dDrawExecutor)

public:
	void reinitialize(PrimitiveType type_, u32 maxInstance_);
	void draw_command() const override;
	void write_to_buffer(Reference<const Rect3d> instance) override;

private:
	PrimitiveType type;

	StructuredBuffer<TransformMatrixDataBuffer> matrices;
	StructuredBuffer<RectData> rectData;
	StructuredBuffer<MaterialDataBuffer4> material;

	std::mutex writeBufferMutex;
};
