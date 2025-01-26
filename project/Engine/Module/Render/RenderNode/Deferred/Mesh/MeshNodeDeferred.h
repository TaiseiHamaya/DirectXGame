#pragma once

#include "Engine/Module/Render/RenderNode/MultiRenderTargetNode.h"

#include "../DeferredAdaptor.h"

class MeshNodeDeferred final : public MultiRenderTargetNode<DeferredAdaptor::NUM_GBUFFER> {
public:
	MeshNodeDeferred();
	~MeshNodeDeferred() noexcept;

	MeshNodeDeferred(const MeshNodeDeferred&) = delete;
	MeshNodeDeferred& operator=(const MeshNodeDeferred&) = delete;
	MeshNodeDeferred(MeshNodeDeferred&&) = default;
	MeshNodeDeferred& operator=(MeshNodeDeferred&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

private:
	void create_pipeline_state();
};
