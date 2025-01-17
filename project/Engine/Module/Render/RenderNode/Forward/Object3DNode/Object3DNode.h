#pragma once

#include "Engine/Module/Render/RenderNode/SingleRenderTargetNode.h"

class Object3DNode final : public SingleRenderTargetNode {
public:
	Object3DNode();
	~Object3DNode() noexcept;

	Object3DNode(const Object3DNode&) = delete;
	Object3DNode& operator=(const Object3DNode&) = delete;
	Object3DNode(Object3DNode&&) = default;
	Object3DNode& operator=(Object3DNode&&) = default;

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
