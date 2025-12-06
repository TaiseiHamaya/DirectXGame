#pragma once

#include "Engine/Module/Render/RenderPipeline/BaseRenderPipeline.h"

namespace szg {

class StaticMeshForwardPipeline final : public BaseRenderPipeline {
public:
	StaticMeshForwardPipeline();
	~StaticMeshForwardPipeline() noexcept;

	StaticMeshForwardPipeline(const StaticMeshForwardPipeline&) = delete;
	StaticMeshForwardPipeline& operator=(const StaticMeshForwardPipeline&) = delete;
	StaticMeshForwardPipeline(StaticMeshForwardPipeline&&) = default;
	StaticMeshForwardPipeline& operator=(StaticMeshForwardPipeline&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

private:
	void create_pipeline_state();
};

}; // szg
