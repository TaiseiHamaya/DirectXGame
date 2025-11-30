#pragma once

#include "Engine/Module/Render/RenderPSO/BaseRenderNode.h"

class SkinningMeshNodeForward final : public BaseRenderNode {
public:
	SkinningMeshNodeForward();
	~SkinningMeshNodeForward() noexcept;

	SkinningMeshNodeForward(const SkinningMeshNodeForward&) = delete;
	SkinningMeshNodeForward& operator=(const SkinningMeshNodeForward&) = delete;
	SkinningMeshNodeForward(SkinningMeshNodeForward&&) = default;
	SkinningMeshNodeForward& operator=(SkinningMeshNodeForward&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

private:
	void create_pipeline_state();

};

