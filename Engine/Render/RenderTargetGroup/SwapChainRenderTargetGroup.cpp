#include "SwapChainRenderTargetGroup.h"

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"

void SwapChainRenderTargetGroup::initialize() {
	create_view_port(WinApp::GetClientWidth(), WinApp::GetClientHight());
	for (int i = 0; i < renderTargets.size(); ++i) {
		renderTargets[i].create_view(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
		renderTargetHandles[i] = renderTargets[i].get_cpu_handle();
	}
}

void SwapChainRenderTargetGroup::finalize() {
}

void SwapChainRenderTargetGroup::set_resource(const Microsoft::WRL::ComPtr<ID3D12Resource>& resource_, std::uint32_t index) {
	renderTargets[index].get_resource() = resource_;
}

const std::array<RenderTarget, SWAPCHAIN_HEAP>& SwapChainRenderTargetGroup::get_render_targets() const {
	return renderTargets;
}

std::array<RenderTarget, SWAPCHAIN_HEAP>& SwapChainRenderTargetGroup::get_render_targets() {
	return renderTargets;
}

void SwapChainRenderTargetGroup::set_render_target(const std::shared_ptr<DepthStencil>& depthStencil) {
	auto&& commandList = DirectXCommand::GetCommandList();
	auto backBufferIndex = DirectXSwapChain::GetBackBufferIndex();
	commandList->OMSetRenderTargets(
		1, &renderTargetHandles[backBufferIndex],
		depthStencil ? 1 : 0,
		depthStencil ? &depthStencil->get_dsv_cpu_handle() : nullptr
	);
}

void SwapChainRenderTargetGroup::clear_render_target() {
	auto backBufferIndex = DirectXSwapChain::GetBackBufferIndex();
	renderTargets[backBufferIndex].clear_resource();
}

void SwapChainRenderTargetGroup::change_render_target_state() {
	renderTargets[DirectXSwapChain::GetBackBufferIndex()].change_resource_state();
}
