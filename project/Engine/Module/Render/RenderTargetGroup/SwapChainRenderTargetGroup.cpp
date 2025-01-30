#include "SwapChainRenderTargetGroup.h"

#include "Engine/Application/EngineSettings.h"
#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/DepthStencil/DepthStencil.h"
#include "Engine/GraphicsAPI/DirectX/DxSystemValues.h"

void SwapChainRenderTargetGroup::initialize() {
	create_view_port(EngineSettings::CLIENT_WIDTH, EngineSettings::CLIENT_HEIGHT);
	for (int i = 0; i < renderTargets.size(); ++i) {
		renderTargets[i].create_view(DxSystemValues::SCREEN_RTV_FORMAT);
		renderTargetHandles[i] = renderTargets[i].get_cpu_handle();
	}
}

void SwapChainRenderTargetGroup::set_resource(const Microsoft::WRL::ComPtr<ID3D12Resource>& resource_, std::uint32_t index) {
	renderTargets[index].get_resource() = resource_;
}

const std::array<RenderTarget, RenderingSystemValues::NUM_BUFFERING>& SwapChainRenderTargetGroup::get_render_targets() const {
	return renderTargets;
}

std::array<RenderTarget, RenderingSystemValues::NUM_BUFFERING>& SwapChainRenderTargetGroup::get_render_targets() {
	return renderTargets;
}

void SwapChainRenderTargetGroup::set_render_target(const std::shared_ptr<DepthStencil>& depthStencil) {
	auto&& commandList = DxCommand::GetCommandList();
	commandList->OMSetRenderTargets(
		1, &renderTargetHandles[RenderingSystemValues::NowBackbufferIndex()],
		depthStencil ? 1 : 0,
		depthStencil ? &depthStencil->get_dsv_cpu_handle() : nullptr
	);
}

void SwapChainRenderTargetGroup::clear_render_target() {
	auto backBufferIndex = RenderingSystemValues::NowBackbufferIndex();
	renderTargets[backBufferIndex].clear_resource();
}

void SwapChainRenderTargetGroup::change_render_target_state() {
	renderTargets[RenderingSystemValues::NowBackbufferIndex()].change_resource_state();
}
