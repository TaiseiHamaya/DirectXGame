#pragma once

#include "Engine/Module/Render/RenderNode/BaseRenderNode.h"

#include <array>

#include "Engine/Module/Render/RenderTargetGroup/MultiRenderTarget.h"

template<u32 NumRenderTarget>
class MultiRenderTargetNode : public BaseRenderNode {
	static_assert(NumRenderTarget >= 2);
public:
	MultiRenderTargetNode() = default;
	virtual ~MultiRenderTargetNode() noexcept = default;

	MultiRenderTargetNode(const MultiRenderTargetNode&) = delete;
	MultiRenderTargetNode& operator=(const MultiRenderTargetNode&) = delete;
	MultiRenderTargetNode(MultiRenderTargetNode&&) = default;
	MultiRenderTargetNode& operator=(MultiRenderTargetNode&&) = default;

public:
	/// <summary>
	/// 描画先の指定
	/// </summary>
	/// <param name="renderTarget_">描画先レンダーターゲットグループ</param>
	virtual void set_render_target(const std::shared_ptr<MultiRenderTarget<NumRenderTarget>>& renderTarget_);

	/// <summary>
	/// 描画結果のRTVHandleの配列
	/// </summary>
	/// <returns></returns>
	const std::array<D3D12_GPU_DESCRIPTOR_HANDLE, NumRenderTarget>& result_stv_handle_list() const;

protected:
	std::array<D3D12_GPU_DESCRIPTOR_HANDLE, NumRenderTarget> resultSrvHandleList;
};

template<u32 NumRenderTarget>
void MultiRenderTargetNode<NumRenderTarget>::set_render_target(const std::shared_ptr<MultiRenderTarget<NumRenderTarget>>& renderTarget_) {
	renderTarget = renderTarget_;
	auto&& list = renderTarget_->offscreen_render_list();
	for (u32 i = 0; i < NumRenderTarget; ++i) {
		resultSrvHandleList[i] = list[i].texture_gpu_handle();
	}
}

template<u32 NumRenderTarget>
const std::array<D3D12_GPU_DESCRIPTOR_HANDLE, NumRenderTarget>& MultiRenderTargetNode<NumRenderTarget>::result_stv_handle_list() const {
	return resultSrvHandleList;
}

/// ----------------------------------
/// ---------- 旧バージョン ----------
/// ----------------------------------

//#include "Engine/Render/RenderNode/BaseRenderNode.h"
//
//class MultiRenderTarget;
//struct D3D12_GPU_DESCRIPTOR_HANDLE;
//
//#include <vector>
//
//class MultiRenderTargetNode : public BaseRenderNode {
//public:
//	MultiRenderTargetNode();
//	virtual ~MultiRenderTargetNode() noexcept;
//
//	MultiRenderTargetNode(const MultiRenderTargetNode&) = delete;
//	MultiRenderTargetNode& operator=(const MultiRenderTargetNode&) = delete;
//	MultiRenderTargetNode(MultiRenderTargetNode&&) = default;
//	MultiRenderTargetNode& operator=(MultiRenderTargetNode&&) = default;
//
//public:
//
//	/// <summary>
//	/// 描画先の指定
//	/// </summary>
//	/// <param name="renderTarget_">描画先レンダーターゲットグループ</param>
//	virtual void set_render_target(const std::shared_ptr<MultiRenderTarget>& renderTarget_);
//
//	/// <summary>
//	/// 描画結果のRTVHandleのvector配列
//	/// </summary>
//	/// <returns></returns>
//	const std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>& result_stv_handle_list() const;
//
//protected:
//	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> resultSrvHandleList;
//};
