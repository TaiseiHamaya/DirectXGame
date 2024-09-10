#pragma once
#include "Engine/Render/RenderTargetGroup/BaseRenderTargetGroup.h"

#include <array>
#include <d3d12.h>

#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Engine/DirectX/DirectXResourceObject/RenderTarget/RenderTarget.h"

class SwapChainRenderTargetGroup final : public BaseRenderTargetGroup {
public:
	SwapChainRenderTargetGroup() = default;
	~SwapChainRenderTargetGroup() = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void finalize() override;

	/// <summary>
	/// resourceの設定
	/// </summary>
	/// <param name="resource_">設定するResource</param>
	/// <param name="index">SwapChainIndex</param>
	void set_resource(const Microsoft::WRL::ComPtr<ID3D12Resource>& resource_, std::uint32_t index);

	/// <summary>
	/// DepthStencilの取得
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<DepthStencil>& get_depth_stencil() const;

	const std::array<RenderTarget, SWAPCHAIN_HEAP>& get_render_targets() const;

private:
	/// <summary>
	/// レンダーターゲットの設定
	/// </summary>
	void set_render_target() override;

	/// <summary>
	/// リソースバリアの状態を変更
	/// </summary>
	void change_render_target_state() override;

private:
	std::array<RenderTarget, SWAPCHAIN_HEAP> renderTargets;
	std::array<D3D12_CPU_DESCRIPTOR_HANDLE, SWAPCHAIN_HEAP> renderTargetHandles{};
};
