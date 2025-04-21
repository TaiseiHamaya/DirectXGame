#include "SingleRenderTarget.h"

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/DepthStencil/DepthStencil.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/OffscreenRender/OffscreenRender.h"

SingleRenderTarget::SingleRenderTarget() = default;

SingleRenderTarget::~SingleRenderTarget() noexcept = default;

void SingleRenderTarget::initialize() {
	initialize(EngineSettings::CLIENT_WIDTH, EngineSettings::CLIENT_HEIGHT);
}

void SingleRenderTarget::initialize(std::uint32_t width, std::uint32_t height) {
	renderTarget = std::make_unique<OffscreenRender>();
	renderTarget->initialize(width, height);
	create_view_port(width, height);
}

const OffscreenRender& SingleRenderTarget::offscreen_render() const {
	return *renderTarget;
}

OffscreenRender& SingleRenderTarget::offscreen_render() {
	return *renderTarget;
}

void SingleRenderTarget::set_render_target(const std::shared_ptr<DepthStencil>& depthStencil) {
	auto&& commandList = DxCommand::GetCommandList();
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
