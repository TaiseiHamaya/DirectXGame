#pragma once

#include "Engine/Module/Render/RenderTargetGroup/BaseRenderTargetGroup.h"

#include <array>

#include <d3d12.h>

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/DepthStencil/DepthStencil.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/OffscreenRender/OffscreenRender.h"

/// <summary>
/// マルチレンダーターゲット
/// </summary>
/// <typeparam name="NumRenderTarget">レンダーターゲット数</typeparam>
template<uint32_t NumRenderTarget = 2>
class MultiRenderTarget final : public BaseRenderTargetGroup {
	static_assert(NumRenderTarget >= 2, "NumRenderTargetは2以上である必要があります");
public:
	MultiRenderTarget() = default;
	~MultiRenderTarget() noexcept = default;;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	/// <summary>
	/// サイズ指定付き初期化
	/// </summary>
	/// <param name="width">幅</param>
	/// <param name="height">高さ</param>
	/// <param name="size">レンダーターゲット数</param>
	void initialize(std::uint32_t width, std::uint32_t height, const std::array<DXGI_FORMAT, NumRenderTarget>& formats);

	std::array<OffscreenRender, NumRenderTarget>& offscreen_render_list();
	const std::array<OffscreenRender, NumRenderTarget>& offscreen_render_list() const;

private:
	/// <summary>
	/// レンダーターゲットの設定
	/// </summary>
	void set_render_target(const std::shared_ptr<DepthStencil>& depthStencil) override;

	/// <summary>
	/// レンダーターゲットのクリア
	/// </summary>
	void clear_render_target() override;

	/// <summary>
	/// リソースバリアの状態を変更
	/// </summary>
	void change_render_target_state() override;

private:
	std::array<OffscreenRender, NumRenderTarget> renderTargets;
	std::array<D3D12_CPU_DESCRIPTOR_HANDLE, NumRenderTarget> renderTargetsHandles;
};

template<uint32_t NumRenderTarget>
inline void MultiRenderTarget<NumRenderTarget>::initialize() {
	initialize(EngineSettings::CLIENT_WIDTH, EngineSettings::CLIENT_HEIGHT, { DXGI_FORMAT_R8G8B8A8_UNORM });
}

template<uint32_t NumRenderTarget>
inline void MultiRenderTarget<NumRenderTarget>::initialize(std::uint32_t width, std::uint32_t height, const std::array<DXGI_FORMAT, NumRenderTarget>& formats) {
	for (uint32_t i = 0; i < NumRenderTarget; ++i) {
		renderTargets[i].initialize(width, height, formats[i]);
		renderTargetsHandles[i] = renderTargets[i].get_cpu_handle();
	}
	create_view_port(width, height);
}

template<uint32_t NumRenderTarget>
inline std::array<OffscreenRender, NumRenderTarget>& MultiRenderTarget<NumRenderTarget>::offscreen_render_list() {
	return renderTargets;
}

template<uint32_t NumRenderTarget>
inline const std::array<OffscreenRender, NumRenderTarget>& MultiRenderTarget<NumRenderTarget>::offscreen_render_list() const {
	return renderTargets;
}

template<uint32_t NumRenderTarget>
inline void MultiRenderTarget<NumRenderTarget>::set_render_target(const std::shared_ptr<DepthStencil>& depthStencil) {
	auto&& commandList = DxCommand::GetCommandList();
	commandList->OMSetRenderTargets(
		static_cast<UINT>(renderTargetsHandles.size()), renderTargetsHandles.data(),
		depthStencil ? 1 : 0,
		depthStencil ? &depthStencil->get_dsv_cpu_handle() : nullptr
	);
}

template<uint32_t NumRenderTarget>
inline void MultiRenderTarget<NumRenderTarget>::clear_render_target() {
	for (OffscreenRender& renderTarget : renderTargets) {
		renderTarget.clear_resource();
	}
}

template<uint32_t NumRenderTarget>
inline void MultiRenderTarget<NumRenderTarget>::change_render_target_state() {
	for (OffscreenRender& renderTarget : renderTargets) {
		renderTarget.change_resource_state();
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
////template<uint32_t NumRenderTarget = 2>
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
//	void initialize(std::uint32_t width, std::uint32_t hight, std::uint32_t size);
//
//	/// <summary>
//	/// サイズ指定付き初期化
//	/// </summary>
//	/// <param name="size">レンダーターゲット数</param>
//	void initialize(std::uint32_t size);
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
