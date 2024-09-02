#include "BaseRenderTargetGroup.h"

#include <d3d12.h>

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXResourceObject/DepthStencil/DepthStencil.h"
#include "Engine/Game/Color/Color.h"

BaseRenderTargetGroup::BaseRenderTargetGroup() = default;

BaseRenderTargetGroup::~BaseRenderTargetGroup() noexcept = default;

void BaseRenderTargetGroup::begin() {
	auto&& commandList = DirectXCommand::GetCommandList();
	// RTのリソースバリアを変更
	change_render_target_state();
	// DepthStencilを持っているならクリアする
	if (depthStencil) {
		commandList->ClearDepthStencilView(
			depthStencil->get_cpu_handle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr
		);
	}
	// ViewPortの設定
	commandList->RSSetViewports(1, viewPort.get());
	// シザー矩形の設定
	commandList->RSSetScissorRects(1, scissorRect.get());
	// レンダーターゲットを指定
	set_render_target();
}

void BaseRenderTargetGroup::end() {
	// RTのリソースバリアを変更
	change_render_target_state();
}

void BaseRenderTargetGroup::set_clear_color(const Color& color_) {
	*clearColor = color_;
}

void BaseRenderTargetGroup::set_depth_stencil(const std::shared_ptr<DepthStencil>& depthStencil_) {
	depthStencil = depthStencil_;
}

bool BaseRenderTargetGroup::has_depth() {
	return depthStencil ? true : false;
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
