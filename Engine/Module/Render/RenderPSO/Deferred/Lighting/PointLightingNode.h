#pragma once

#include "Engine/Module/Render/RenderPSO/BaseRenderNode.h"

#include "../DeferredAdaptor.h"

class PointLightingNode final : public BaseRenderNode {
public:
	PointLightingNode();
	~PointLightingNode() noexcept;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override;

	void set_gbuffers(std::array<Reference<RenderTexture>, DeferredAdaptor::NUM_GBUFFER> gBufferTextures_);

private:
	void create_pipeline_state();

private:
	std::array<Reference<RenderTexture>, DeferredAdaptor::NUM_GBUFFER> gBufferTextures;
	Reference<DepthStencilTexture> depthTexture;
};

