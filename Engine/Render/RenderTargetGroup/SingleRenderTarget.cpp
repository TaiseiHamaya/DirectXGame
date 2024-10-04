#include "SingleRenderTarget.h"

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXResourceObject/DepthStencil/DepthStencil.h"
#include "Engine/DirectX/DirectXResourceObject/OffscreenRender/OffscreenRender.h"

SingleRenderTarget::SingleRenderTarget() = default;

SingleRenderTarget::~SingleRenderTarget() noexcept = default;

void SingleRenderTarget::initialize() {
	initialize(WinApp::GetClientWidth(), WinApp::GetClientHight());
}

void SingleRenderTarget::finalize() {
	renderTarget->release_index();
}

void SingleRenderTarget::initialize(std::uint32_t width, std::uint32_t hight) {
	renderTarget = std::make_unique<OffscreenRender>();
	renderTarget->initialize(width, hight);
	create_view_port(width, hight);
}

const OffscreenRender& SingleRenderTarget::main_offscreen_render() {
	return *renderTarget;
}

void SingleRenderTarget::set_render_target(const std::shared_ptr<DepthStencil>& depthStencil) {
	auto&& commandList = DirectXCommand::GetCommandList();
	commandList->OMSetRenderTargets(
		1, &renderTarget->get_cpu_handle(),
		depthStencil ? 1 : 0,
		depthStencil ? &depthStencil->get_dsv_cpu_handle() : nullptr
	);
}

void SingleRenderTarget::clear_render_target() {
	renderTarget->clear_resource();
}

void SingleRenderTarget::change_render_target_state() {
	renderTarget->change_resource_state();
}
