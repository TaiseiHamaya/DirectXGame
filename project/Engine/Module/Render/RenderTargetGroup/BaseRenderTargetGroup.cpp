#include "BaseRenderTargetGroup.h"

#include <d3d12.h>

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/DepthStencil/DepthStencil.h"
#include "Engine/Module/Render/RenderNode/BaseRenderNode.h"

BaseRenderTargetGroup::BaseRenderTargetGroup() = default;

BaseRenderTargetGroup::~BaseRenderTargetGroup() noexcept = default;

void BaseRenderTargetGroup::begin(const eps::bitflag<RenderNodeConfig>& config_, const std::shared_ptr<DepthStencil>& depthStencil) {
	auto&& commandList = DxCommand::GetCommandList();
	// RTのリソースバリアを変更
	if (!(config_ & RenderNodeConfig::NoChangeStateBegin)) {
		change_render_target_state();
	}
	// ViewPortの設定
	commandList->RSSetViewports(1, viewPort.get());
	// シザー矩形の設定
	commandList->RSSetScissorRects(1, scissorRect.get());
	// レンダーターゲットを指定
	set_render_target(depthStencil);
	// レンダーターゲットのクリア
	if (!(config_ & RenderNodeConfig::NoClearRenderTarget)) {
		clear_render_target();
	}
}

void BaseRenderTargetGroup::end(const eps::bitflag<RenderNodeConfig>& config_) {
	// RTのリソースバリアを変更
	if (!(config_ & RenderNodeConfig::NoChangeStateEnd)) {
		change_render_target_state();
	}
}

void BaseRenderTargetGroup::create_view_port(std::uint32_t width, std::uint32_t height) {
	viewPort = std::make_unique<D3D12_VIEWPORT>(
		0.0f, 0.0f,
		static_cast<FLOAT>(width),
		static_cast<FLOAT>(height),
		0.0f, 1.0f
	);
	scissorRect = std::make_unique<D3D12_RECT>(
		0, 0, width, height
	);
}
