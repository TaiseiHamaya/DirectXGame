#pragma once

#include "Engine/Module/Render/RenderNode/BaseRenderNode.h"

class SingleRenderTarget;
class SwapChainRenderTargetGroup;

class SingleRenderTargetNode : public BaseRenderNode {
public:
	SingleRenderTargetNode();
	virtual ~SingleRenderTargetNode() noexcept;

	SingleRenderTargetNode(const SingleRenderTargetNode&) = delete;
	SingleRenderTargetNode& operator=(const SingleRenderTargetNode&) = delete;
	SingleRenderTargetNode(SingleRenderTargetNode&&) = default;
	SingleRenderTargetNode& operator=(SingleRenderTargetNode&&) = default;

public:
	/// <summary>
	/// 描画先の指定
	/// </summary>
	/// <param name="renderTarget_">描画先レンダーターゲットグループ</param>
	void set_render_target(Reference<SingleRenderTarget> renderTargetGroup);

	/// <summary>
	/// 描画先の指定
	/// </summary>
	void set_render_target_SC();
};
