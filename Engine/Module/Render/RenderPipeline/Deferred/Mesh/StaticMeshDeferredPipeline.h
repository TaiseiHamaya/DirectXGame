#pragma once

#include "Engine/Module/Render/RenderPipeline/BaseRenderPipeline.h"

class StaticMeshDeferredPipeline final : public BaseRenderPipeline {
public:
	StaticMeshDeferredPipeline();
	~StaticMeshDeferredPipeline() noexcept;

	StaticMeshDeferredPipeline(const StaticMeshDeferredPipeline&) = delete;
	StaticMeshDeferredPipeline& operator=(const StaticMeshDeferredPipeline&) = delete;
	StaticMeshDeferredPipeline(StaticMeshDeferredPipeline&&) = default;
	StaticMeshDeferredPipeline& operator=(StaticMeshDeferredPipeline&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

private:
	void create_pipeline_state();
};
