#pragma once

#include <vector>
#include <mutex>

#include "../BaseDrawExecutor.h"

#include <Library/Utility/Template/Reference.h>

#include "Engine/Assets/PolygonMesh/PolygonMesh.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/StructuredBuffer/StructuredBuffer.h"

class StaticMeshInstance;
struct TransformMatrixData;
struct MaterialBufferData;

class StaticMeshDrawExecutor final : public BaseDrawExecutor {
public:
	StaticMeshDrawExecutor() noexcept = default;
	 ~StaticMeshDrawExecutor() noexcept = default;

	StaticMeshDrawExecutor(std::shared_ptr<const PolygonMesh> asset_, uint32_t maxInstance);

	__NON_COPYABLE_CLASS(StaticMeshDrawExecutor)

public:
	void reinitialize(std::shared_ptr<const PolygonMesh> asset_, uint32_t maxInstance);
	void draw_command() const override;
	void write_to_buffer(Reference<const StaticMeshInstance> instance);

private:
	std::shared_ptr<const PolygonMesh> asset;

	StructuredBuffer<TransformMatrixData> matrices;
	std::vector<StructuredBuffer<MaterialBufferData>> materials;

	std::mutex writeBufferMutex;
};
