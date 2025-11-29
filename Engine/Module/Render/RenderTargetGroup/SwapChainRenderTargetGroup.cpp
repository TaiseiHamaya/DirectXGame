#include "SwapChainRenderTargetGroup.h"

#include "Engine/Application/ProjectSettings/ProjectSettings.h"
#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"

void SwapChainRenderTargetGroup::initialize() {
	textures.resize(ProjectSettings::GetGraphicsSettingsImm().numBuffering);
	create_view_port(ProjectSettings::ClientWidth(), ProjectSettings::ClientHeight());
}

void SwapChainRenderTargetGroup::set_resource(Reference<ScreenTexture> texture, u32 index) {
	textures[index] = texture;
}

void SwapChainRenderTargetGroup::set_clear_color(const Color4& color) noexcept {
	clearColor = color;
}

void SwapChainRenderTargetGroup::start_render_target(Reference<DepthStencilTexture> depthStencil) {
	u32 backBufferIndex = RenderingSystemValues::NowBackbufferIndex();
	textures[backBufferIndex]->start_write();
	auto&& commandList = DxCommand::GetCommandList();
	commandList->OMSetRenderTargets(
		1, &textures[backBufferIndex]->get_as_rtv()->handle(),
		depthStencil ? 1 : 0,
		depthStencil ? &depthStencil->get_as_dsv()->handle() : nullptr
	);
}

void SwapChainRenderTargetGroup::clear_render_target() {
	u32 backBufferIndex = RenderingSystemValues::NowBackbufferIndex();
	textures[backBufferIndex]->get_as_rtv()->clear(clearColor);
}
