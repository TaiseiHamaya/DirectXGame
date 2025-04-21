#pragma once

#include "Engine/Module/Render/RenderNode/SingleRenderTargetNode.h"

class SkinningMeshNodeForward final : public SingleRenderTargetNode {
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

	void set_render_target(const std::shared_ptr<SingleRenderTarget>& renderTarget_ = nullptr) override;

private:
	void create_pipeline_state();

};

