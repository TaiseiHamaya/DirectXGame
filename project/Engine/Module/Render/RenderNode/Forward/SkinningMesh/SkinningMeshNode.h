#pragma once

#include "Engine/Module/Render/RenderNode/SingleRenderTargetNode.h"

class SkinningMeshNode final : public SingleRenderTargetNode {
public:
	SkinningMeshNode();
	~SkinningMeshNode() noexcept;

	SkinningMeshNode(const SkinningMeshNode&) = delete;
	SkinningMeshNode& operator=(const SkinningMeshNode&) = delete;
	SkinningMeshNode(SkinningMeshNode&&) = default;
	SkinningMeshNode& operator=(SkinningMeshNode&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	//void use() override;

	void set_render_target(const std::shared_ptr<SingleRenderTarget>& renderTarget_ = nullptr) override;

private:
	void create_pipeline_state();

};

