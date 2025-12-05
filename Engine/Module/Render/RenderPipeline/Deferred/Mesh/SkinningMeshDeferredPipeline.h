#pragma once

#include "Engine/Module/Render/RenderPipeline/BaseRenderPipeline.h"

class SkinningMeshDeferredPipeline final : public BaseRenderPipeline {
public:
	SkinningMeshDeferredPipeline();
	~SkinningMeshDeferredPipeline() noexcept;

	SkinningMeshDeferredPipeline(const SkinningMeshDeferredPipeline&) = delete;
	SkinningMeshDeferredPipeline& operator=(const SkinningMeshDeferredPipeline&) = delete;
	SkinningMeshDeferredPipeline(SkinningMeshDeferredPipeline&&) = default;
	SkinningMeshDeferredPipeline& operator=(SkinningMeshDeferredPipeline&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

private:
	void create_pipeline_state();
};

