#pragma once

#include "Engine/Module/Render/RenderNode/MultiRenderTargetNode.h"

#include "../DeferredAdaptor.h"

class StaticMeshNodeDeferred final : public MultiRenderTargetNode<DeferredAdaptor::NUM_GBUFFER> {
public:
	StaticMeshNodeDeferred();
	~StaticMeshNodeDeferred() noexcept;

	StaticMeshNodeDeferred(const StaticMeshNodeDeferred&) = delete;
	StaticMeshNodeDeferred& operator=(const StaticMeshNodeDeferred&) = delete;
	StaticMeshNodeDeferred(StaticMeshNodeDeferred&&) = default;
	StaticMeshNodeDeferred& operator=(StaticMeshNodeDeferred&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

private:
	void create_pipeline_state();
};
