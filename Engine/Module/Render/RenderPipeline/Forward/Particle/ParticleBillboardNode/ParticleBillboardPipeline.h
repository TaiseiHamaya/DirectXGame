#pragma once

#include "Engine/Module/Render/RenderPipeline/BaseRenderPipeline.h"

namespace szg {

class ParticleBillboardPipeline : public BaseRenderPipeline {
public:
	ParticleBillboardPipeline();
	~ParticleBillboardPipeline() noexcept;

	ParticleBillboardPipeline(const ParticleBillboardPipeline&) = delete;
	ParticleBillboardPipeline& operator=(const ParticleBillboardPipeline&) = delete;
	ParticleBillboardPipeline(ParticleBillboardPipeline&&) = default;
	ParticleBillboardPipeline& operator=(ParticleBillboardPipeline&&) = default;

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
