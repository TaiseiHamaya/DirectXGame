#pragma once

#include "Engine/Module/Render/RenderNode/SingleRenderTargetNode.h"

#include "../DeferredAdaptor.h"

class RenderTexture;
class DepthStencilTexture;

class DirectionalLightingNode final : public SingleRenderTargetNode {
public:
	DirectionalLightingNode();
	~DirectionalLightingNode() noexcept;

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
