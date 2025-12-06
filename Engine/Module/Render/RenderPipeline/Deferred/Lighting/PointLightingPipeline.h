#pragma once

#include "Engine/Module/Render/RenderPipeline/BaseRenderPipeline.h"

#include "../DeferredAdaptor.h"

namespace szg {

class PointLightingPipeline final : public BaseRenderPipeline {
public:
	PointLightingPipeline();
	~PointLightingPipeline() noexcept;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

	void stack_lighting_command();

	void set_gbuffers(std::array<Reference<RenderTexture>, DeferredAdaptor::NUM_GBUFFER> gBufferTextures_);

private:
	void create_pipeline_state();

private:
	std::array<Reference<RenderTexture>, DeferredAdaptor::NUM_GBUFFER> gBufferTextures;
	Reference<DepthStencilTexture> depthTexture;
};


}; // szg
