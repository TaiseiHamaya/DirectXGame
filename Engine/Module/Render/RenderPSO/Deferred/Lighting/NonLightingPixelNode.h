#pragma once

#include "Engine/Module/Render/RenderNode/SingleRenderTargetNode.h"

#include "../DeferredAdaptor.h"

class NonLightingPixelNode final : public SingleRenderTargetNode {
public:
	NonLightingPixelNode();
	~NonLightingPixelNode() noexcept;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override;

	void set_gbuffers(Reference<RenderTexture> gBufferColorTexture_);

private:
	void create_pipeline_state();

private:
	Reference<RenderTexture> gBufferColorTexture;
};
