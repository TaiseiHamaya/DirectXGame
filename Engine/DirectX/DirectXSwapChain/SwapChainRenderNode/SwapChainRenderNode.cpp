#include "SwapChainRenderNode.h"

#include "Engine/DirectX/PipelineState/PipelineState.h"
#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/WinApp.h"

void SwapChainRenderNode::initialize() {
	renderTargetHandles.clear();
	pipelineState = nullptr;
	depthStencil = std::make_unique<DepthStencil>();
	depthStencil->initialize();
	create_view_port(WinApp::GetClientWidth(), WinApp::GetClientHight());
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	clearColor = std::make_unique<Color>(0.1f, 0.25f, 0.5f, 1.0f);
}

void SwapChainRenderNode::begin() {
	auto&& commandList = DirectXCommand::GetCommandList();
	auto backBufferIndex = DirectXSwapChain::GetBackBufferIndex();
	commandList->OMSetRenderTargets(
		1, &renderTargetHandles[backBufferIndex], 
		1, &depthStencil->get_cpu_handle()
	);
	commandList->ClearRenderTargetView(
		renderTargetHandles[backBufferIndex],
		&clearColor->red, 0, nullptr
	);
	BaseRenderNode::begin();
}

void SwapChainRenderNode::set_pso(std::unique_ptr<PipelineState>&& piplineState_) {
	pipelineState = std::move(piplineState_);
}

void SwapChainRenderNode::set_render_target(const std::array<RenderTarget, SWAPCHAIN_HEAP>& renderTargets) {
	renderTargetHandles.resize(renderTargets.size());
	for (int i = 0; i < renderTargetHandles.size(); ++i) {
		renderTargetHandles[i] = renderTargets[i].get_cpu_handle();
	}
}

void SwapChainRenderNode::set_clear_color(const Color& color_) {
	*clearColor = color_;
}

DepthStencil& SwapChainRenderNode::get_depth_stencil() {
	return *depthStencil;
}
