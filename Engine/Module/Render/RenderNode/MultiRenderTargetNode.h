#pragma once

#include "Engine/Module/Render/RenderNode/BaseRenderNode.h"

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
	void set_render_target(Reference<MultiRenderTarget<NumRenderTarget>> renderTarget_);
};

template<u32 NumRenderTarget>
void MultiRenderTargetNode<NumRenderTarget>::set_render_target(Reference<MultiRenderTarget<NumRenderTarget>> renderTarget_) {
	renderTarget = renderTarget_;
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
