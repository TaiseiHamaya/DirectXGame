#pragma once

#include "Engine/Module/Render/RenderNode/MultiRenderTargetNode.h"

#include "../DeferredAdaptor.h"

class SkinningMeshNodeDeferred final : public MultiRenderTargetNode<DeferredAdaptor::NUM_GBUFFER> {
public:
	SkinningMeshNodeDeferred();
	~SkinningMeshNodeDeferred() noexcept;

	SkinningMeshNodeDeferred(const SkinningMeshNodeDeferred&) = delete;
	SkinningMeshNodeDeferred& operator=(const SkinningMeshNodeDeferred&) = delete;
	SkinningMeshNodeDeferred(SkinningMeshNodeDeferred&&) = default;
	SkinningMeshNodeDeferred& operator=(SkinningMeshNodeDeferred&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

private:
	void create_pipeline_state();
};

