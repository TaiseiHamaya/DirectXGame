#pragma once

#include "Engine/Module/Render/RenderPSO/BaseRenderNode.h"

class SkinningMeshNodeDeferred final : public BaseRenderNode {
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

