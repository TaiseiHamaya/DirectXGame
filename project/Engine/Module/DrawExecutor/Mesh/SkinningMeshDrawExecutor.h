#pragma once

#include <vector>
#include <mutex>

#include "../BaseDrawExecutor.h"

#include <Library/Utility/Template/Reference.h>

#include "Engine/Assets/PolygonMesh/PolygonMesh.h"
#include "Engine/Assets/Animation/Skeleton/SkeletonAsset.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/StructuredBuffer/StructuredBuffer.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/StructuredBuffer/MdStructuredBuffer.h"

class SkinningMeshInstance;
struct TransformMatrixData;
struct MaterialBufferData;
struct SkeletonMatrixPaletteWell;

class SkinningMeshDrawExecutor final : public BaseDrawExecutor {
public:
	SkinningMeshDrawExecutor() noexcept = default;
	~SkinningMeshDrawExecutor() noexcept = default;

	SkinningMeshDrawExecutor(std::shared_ptr<const PolygonMesh> mesh, std::shared_ptr<const SkeletonAsset> skeleton, uint32_t maxInstance);
	
	__NON_COPYABLE_CLASS(SkinningMeshDrawExecutor)

public:
	void reinitialize(std::shared_ptr<const PolygonMesh> mesh, std::shared_ptr<const SkeletonAsset> skeleton, uint32_t maxInstance);
	void draw_command() const override;
	void write_to_buffer(Reference<const SkinningMeshInstance> instance);

private:
	std::shared_ptr<const PolygonMesh> mesh;
	std::shared_ptr<const SkeletonAsset> skeletonData;
	
	StructuredBuffer<TransformMatrixData> matrices;
	std::vector<StructuredBuffer<MaterialBufferData>> materials;
	std::vector<MdStructuredBuffer<SkeletonMatrixPaletteWell>> matrixPalettes; // GPU用Matrix
	std::vector<ConstantBuffer<uint32_t>> paletteSize;

	std::mutex writeBufferMutex;
};
