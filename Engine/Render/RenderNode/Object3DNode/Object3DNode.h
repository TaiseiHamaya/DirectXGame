#pragma once

#include "Engine/Render/RenderNode/SingleRenderTargetNode.h"

class DepthStencil;

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

	void begin() override;

	//void use() override;


private:
	void create_pipeline_state();
};
