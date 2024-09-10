#include "SingleRenderTarget.h"

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXResourceObject/OffscreenRender/OffscreenRender.h"
#include "Engine/Game/Color/Color.h"
#include "Engine/DirectX/DirectXResourceObject/DepthStencil/DepthStencil.h"

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
	clearColor = std::make_unique<Color>(0.1f, 0.25f, 0.5f, 1.0f);
}

const OffscreenRender& SingleRenderTarget::main_offscreen_render() {
	return *renderTarget;
}

void SingleRenderTarget::set_render_target() {
	auto&& commandList = DirectXCommand::GetCommandList();
	commandList->OMSetRenderTargets(
		1, &renderTarget->get_cpu_handle(),
		depthStencil ? 1 : 0,
		depthStencil ? &depthStencil->get_cpu_handle() : nullptr
	);
	commandList->ClearRenderTargetView(
		renderTarget->get_cpu_handle(),
		&clearColor->red, 0, nullptr
	);
}

void SingleRenderTarget::change_render_target_state() {
	renderTarget->change_resource_state();
}
