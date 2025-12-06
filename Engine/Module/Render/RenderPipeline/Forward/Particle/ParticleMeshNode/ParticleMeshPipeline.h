#pragma once

#include "Engine/Module/Render/RenderPipeline/BaseRenderPipeline.h"

namespace szg {

class ParticleMeshPipeline final : public BaseRenderPipeline {
public:
	ParticleMeshPipeline();
	~ParticleMeshPipeline() noexcept;

	ParticleMeshPipeline(const ParticleMeshPipeline&) = delete;
	ParticleMeshPipeline& operator=(const ParticleMeshPipeline&) = delete;
	ParticleMeshPipeline(ParticleMeshPipeline&&) = default;
	ParticleMeshPipeline& operator=(ParticleMeshPipeline&&) = default;

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
