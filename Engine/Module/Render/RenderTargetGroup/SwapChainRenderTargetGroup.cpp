#include "SwapChainRenderTargetGroup.h"

#include "Engine/Application/EngineSettings.h"
#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"

void SwapChainRenderTargetGroup::initialize() {
	create_view_port(EngineSettings::CLIENT_WIDTH, EngineSettings::CLIENT_HEIGHT);
}

void SwapChainRenderTargetGroup::set_resource(Reference<ScreenTexture> texture, u32 index) {
	textures[index] = texture;
}
void SwapChainRenderTargetGroup::start_render_target(Reference<DepthStencilTexture> depthStencil) {
	textures[RenderingSystemValues::NowBackbufferIndex()]->start_write();
	auto&& commandList = DxCommand::GetCommandList();
	commandList->OMSetRenderTargets(
		1, &textures[RenderingSystemValues::NowBackbufferIndex()]->get_as_rtv()->handle(),
		depthStencil ? 1 : 0,
		depthStencil ? &depthStencil->get_as_dsv()->handle() : nullptr
	);
}

void SwapChainRenderTargetGroup::clear_render_target() {
	auto backBufferIndex = RenderingSystemValues::NowBackbufferIndex();
	textures[backBufferIndex]->get_as_rtv()->clear(clearColor);
}
