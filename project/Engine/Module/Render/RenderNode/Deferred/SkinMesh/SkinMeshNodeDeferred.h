#pragma once

#include "Engine/Module/Render/RenderNode/MultiRenderTargetNode.h"

#include "../DeferredAdaptor.h"

class SkinMeshNodeDeferred final : public MultiRenderTargetNode<DeferredAdaptor::NUM_GBUFFER> {
public:
	SkinMeshNodeDeferred();
	~SkinMeshNodeDeferred() noexcept;

	SkinMeshNodeDeferred(const SkinMeshNodeDeferred&) = delete;
	SkinMeshNodeDeferred& operator=(const SkinMeshNodeDeferred&) = delete;
	SkinMeshNodeDeferred(SkinMeshNodeDeferred&&) = default;
	SkinMeshNodeDeferred& operator=(SkinMeshNodeDeferred&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

private:
	void create_pipeline_state();
};

