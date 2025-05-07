#pragma once

#include "Engine/Module/Render/RenderTargetGroup/BaseRenderTargetGroup.h"

#include <array>

#include <d3d12.h>

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/DepthStencilTexture.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h"

/// <summary>
/// マルチレンダーターゲット
/// </summary>
/// <typeparam name="NumRenderTarget">レンダーターゲット数</typeparam>
template<u32 NumRenderTarget = 2>
class MultiRenderTarget final : public BaseRenderTargetGroup {
	static_assert(NumRenderTarget >= 2, "NumRenderTargetは2以上である必要があります");
public:
	MultiRenderTarget() = default;
	~MultiRenderTarget() noexcept = default;;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="renderTextures_"></param>
	void initialize(std::array<Reference<RenderTexture>, NumRenderTarget> renderTextures_);

private:
	void start_render_target(Reference<DepthStencilTexture> depthStencil) override;

	/// <summary>
	/// レンダーターゲットのクリア
	/// </summary>
	void clear_render_target() override;

private:
	std::array<Reference<RenderTexture>, NumRenderTarget> renderTextures;
	std::array<D3D12_CPU_DESCRIPTOR_HANDLE, NumRenderTarget> renderTargetsHandles;
};

template<u32 NumRenderTarget>
inline void MultiRenderTarget<NumRenderTarget>::initialize(std::array<Reference<RenderTexture>, NumRenderTarget> renderTextures_) {
	renderTextures = renderTextures_;
	for (u32 i = 0; i < NumRenderTarget; ++i) {
		renderTargetsHandles[i] = renderTextures[i]->get_as_rtv()->handle();
	}
	create_view_port(renderTextures[0]->get_width(), renderTextures[0]->get_height());
}

template<u32 NumRenderTarget>
inline void MultiRenderTarget<NumRenderTarget>::start_render_target(Reference<DepthStencilTexture> depthStencil) {
	for (Reference<RenderTexture>& renderTarget : renderTextures) {
		renderTarget->start_write();
	}
	auto&& commandList = DxCommand::GetCommandList();
	commandList->OMSetRenderTargets(
		static_cast<UINT>(renderTargetsHandles.size()), renderTargetsHandles.data(),
		depthStencil ? 1 : 0,
		depthStencil ? &depthStencil->get_as_dsv()->handle() : nullptr
	);
}

template<u32 NumRenderTarget>
inline void MultiRenderTarget<NumRenderTarget>::clear_render_target() {
	for (Reference<RenderTexture>& renderTarget : renderTextures) {
		renderTarget->get_as_rtv()->clear(clearColor);
	}
}

/// ----------------------------------
/// ---------- 旧バージョン ----------
/// ----------------------------------

//#include "Engine/Render/RenderTargetGroup/BaseRenderTargetGroup.h"
//
//#include <vector>
//
//class OffscreenRender;
//struct D3D12_CPU_DESCRIPTOR_HANDLE;
//
////template<u32 NumRenderTarget = 2>
//class MultiRenderTarget final : public BaseRenderTargetGroup {
//	//static_assert(NumRenderTarget >= 2, "NumRenderTargetは2以上である必要があります");
//public:
//	MultiRenderTarget();
//	~MultiRenderTarget() noexcept;
//
//public:
//	/// <summary>
//	/// 初期化
//	/// </summary>
//	void initialize() override;
//
//	/// <summary>
//	/// 終了処理
//	/// </summary>
//	void finalize() override;
//
//	/// <summary>
//	/// サイズ指定付き初期化
//	/// </summary>
//	/// <param name="width">幅</param>
//	/// <param name="hight">高さ</param>
//	/// <param name="size">レンダーターゲット数</param>
//	void initialize(u32 width, u32 hight, u32 size);
//
//	/// <summary>
//	/// サイズ指定付き初期化
//	/// </summary>
//	/// <param name="size">レンダーターゲット数</param>
//	void initialize(u32 size);
//
//	std::vector<OffscreenRender>& offscreen_render_list();
//	const std::vector<OffscreenRender>& offscreen_render_list() const;
//
//private:
//	/// <summary>
//	/// レンダーターゲットの設定
//	/// </summary>
//	void set_render_target(const std::shared_ptr<DepthStencil>& depthStencil) override;
//
//	/// <summary>
//	/// レンダーターゲットのクリア
//	/// </summary>
//	void clear_render_target() override;
//
//	/// <summary>
//	/// リソースバリアの状態を変更
//	/// </summary>
//	void change_render_target_state() override;
//
//private:
//	std::vector<OffscreenRender> renderTargets;
//	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> renderTargetsHandles;
//};
