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

	/// <summary>
	/// 描画開始処理(ライトの設定があるのでovrride)
	/// </summary>
	void begin() override;

	/// <summary>
	/// 描画先の指定(depthの指定などがあるのでoverride)
	/// </summary>
	/// <param name="renderTarget_"></param>
	void set_render_target(const std::shared_ptr<SingleRenderTarget>& renderTarget_ = nullptr) override;

	void set_depth_stencil();

private:
	void create_pipeline_state();

private:
};
