#pragma once

#include "Engine/Module/Render/RenderNode/SingleRenderTargetNode.h"

#include "../DeferredAdaptor.h"

/// 既知の不具合一覧 
/// ・ライト内にカメラがあった際に、正しくライティングされない。
/// ・ライト位置に対して背面を向いている面でライティングされる場合がある。

class PointLightingNode final : public SingleRenderTargetNode {
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

