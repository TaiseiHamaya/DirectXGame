#pragma once

#include "Engine/Module/Render/RenderPipeline/BaseRenderPipeline.h"

#include "../DeferredAdaptor.h"

class NonLightingPixelPipeline final : public BaseRenderPipeline {
public:
	NonLightingPixelPipeline();
	~NonLightingPixelPipeline() noexcept;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

	void stack_lighting_command();

	void set_gbuffers(Reference<RenderTexture> gBufferColorTexture_);

private:
	void create_pipeline_state();

private:
	Reference<RenderTexture> gBufferColorTexture;
};
