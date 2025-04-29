#include "BaseRenderTargetGroup.h"

#include <d3d12.h>

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/Module/Render/RenderNode/BaseRenderNode.h"

BaseRenderTargetGroup::BaseRenderTargetGroup() = default;

BaseRenderTargetGroup::~BaseRenderTargetGroup() noexcept = default;

void BaseRenderTargetGroup::begin_write(const eps::bitflag<RenderNodeConfig>& config, Reference<DepthStencilTexture> depthStencil) {
	auto&& commandList = DxCommand::GetCommandList();
	// レンダーターゲットを指定
	start_render_target(depthStencil);
	// ViewPortの設定
	commandList->RSSetViewports(1, viewPort.get());
	// シザー矩形の設定
	commandList->RSSetScissorRects(1, scissorRect.get());
	// レンダーターゲットのクリア
	if (!(config & RenderNodeConfig::NoClearRenderTarget)) {
		clear_render_target();
	}
}

void BaseRenderTargetGroup::create_view_port(u32 width, u32 height) {
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
