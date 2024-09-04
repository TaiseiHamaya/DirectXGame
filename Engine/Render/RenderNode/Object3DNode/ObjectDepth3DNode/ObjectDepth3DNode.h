#pragma once

#include "Engine/Render/RenderNode/MultiRenderTargetNode.h"

class ObjectDepth3DNode final : public MultiRenderTargetNode {
public:
	ObjectDepth3DNode();
	virtual ~ObjectDepth3DNode() noexcept;

	ObjectDepth3DNode(const ObjectDepth3DNode&) = delete;
	ObjectDepth3DNode& operator=(const ObjectDepth3DNode&) = delete;
	ObjectDepth3DNode(ObjectDepth3DNode&&) = default;
	ObjectDepth3DNode& operator=(ObjectDepth3DNode&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	/// <summary>
	/// 描画開始処理(ライトの設定があるのでoverride)
	/// </summary>
	void begin() override;

	/// <summary>
	/// 描画先の指定(depthの指定などがあるのでoverride)
	/// </summary>
	/// <param name="renderTarget_"></param>
	void set_render_target(const std::shared_ptr<MultiRenderTarget>& renderTarget_ = nullptr) override;

private:
	void create_pipeline_state();

private:
};
