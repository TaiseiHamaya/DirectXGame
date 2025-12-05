#pragma once

#include "Engine/Module/Render/RenderPipeline/BaseRenderPipeline.h"

#include "../DeferredAdaptor.h"

class RenderTexture;
class DepthStencilTexture;

class DirectionalLightingPipeline final : public BaseRenderPipeline {
public:
	DirectionalLightingPipeline();
	~DirectionalLightingPipeline() noexcept;

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
