#pragma once

#include "Engine/Module/Render/RenderTargetGroup/BaseRenderTargetGroup.h"

#include <array>
#include <d3d12.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/RenderTarget/RenderTarget.h"
#include "Engine/GraphicsAPI/RenderingSystemValues.h"

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
	/// resourceの設定
	/// </summary>
	/// <param name="resource_">設定するResource</param>
	/// <param name="index">SwapChainIndex</param>
	void set_resource(const Microsoft::WRL::ComPtr<ID3D12Resource>& resource_, std::uint32_t index);

	const std::array<RenderTarget, RenderingSystemValues::NUM_BUFFERING>& get_render_targets() const;
	std::array<RenderTarget, RenderingSystemValues::NUM_BUFFERING>& get_render_targets();

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
	std::array<RenderTarget, RenderingSystemValues::NUM_BUFFERING> renderTargets;
	std::array<D3D12_CPU_DESCRIPTOR_HANDLE, RenderingSystemValues::NUM_BUFFERING> renderTargetHandles{};
};
