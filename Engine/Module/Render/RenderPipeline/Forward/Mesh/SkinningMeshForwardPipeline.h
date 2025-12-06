#pragma once

#include "Engine/Module/Render/RenderPipeline/BaseRenderPipeline.h"

namespace szg {

class SkinningMeshForwardPipeline final : public BaseRenderPipeline {
public:
	SkinningMeshForwardPipeline();
	~SkinningMeshForwardPipeline() noexcept;

	SkinningMeshForwardPipeline(const SkinningMeshForwardPipeline&) = delete;
	SkinningMeshForwardPipeline& operator=(const SkinningMeshForwardPipeline&) = delete;
	SkinningMeshForwardPipeline(SkinningMeshForwardPipeline&&) = default;
	SkinningMeshForwardPipeline& operator=(SkinningMeshForwardPipeline&&) = default;

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
