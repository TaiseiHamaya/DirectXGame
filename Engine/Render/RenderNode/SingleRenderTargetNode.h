#pragma once

#include "Engine/Render/RenderNode/BaseRenderNode.h"

class SingleRenderTarget;
class SwapChainRenderTargetGroup;

#include <d3d12.h>
#include <optional>

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
	/// 初期化
	/// </summary>
	virtual void initialize() override = 0;

	/// <summary>
	/// 描画先の指定
	/// </summary>
	/// <param name="renderTarget_">描画先レンダーターゲットグループ</param>
	virtual void set_render_target(const std::shared_ptr<SingleRenderTarget>& renderTarget_ = nullptr);

	/// <summary>
	/// 描画先の指定
	/// </summary>
	/// <param name="renderTarget_">描画先レンダーターゲットグループ</param>
	void set_render_target_SC(const std::shared_ptr<SwapChainRenderTargetGroup>& renderTarget_);

	/// <summary>
	/// 描画結果のRTVHandle
	/// </summary>
	/// <returns></returns>
	const D3D12_GPU_DESCRIPTOR_HANDLE& result_stv_handle() const;

protected:
	std::optional<D3D12_GPU_DESCRIPTOR_HANDLE> resultSvtHandle;
};
