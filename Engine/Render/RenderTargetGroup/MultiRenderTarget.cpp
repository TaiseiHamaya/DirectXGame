#include "MultiRenderTarget.h"

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXResourceObject/OffscreenRender/OffscreenRender.h"
#include "Engine/Game/Color/Color.h"
#include "Engine/DirectX/DirectXResourceObject/DepthStencil/DepthStencil.h"
#include <d3d12.h>

#include <format>

MultiRenderTarget::MultiRenderTarget() = default;

MultiRenderTarget::~MultiRenderTarget() noexcept = default;

void MultiRenderTarget::initialize() {
	initialize(WinApp::GetClientWidth(), WinApp::GetClientHight(), 1);
}

void MultiRenderTarget::finalize() {
	for (auto& renderTarget : renderTargets) {
		renderTarget.release_index();
	}
}

void MultiRenderTarget::initialize(std::uint32_t width, std::uint32_t hight, std::uint32_t size) {
	renderTargets.resize(size);
	for (auto itr = renderTargets.begin(); itr < renderTargets.end(); ++itr) {
		itr->initialize(width, hight);
		renderTargetsHandles.emplace_back(itr->get_cpu_handle());
	}
	create_view_port(width, hight);
	clearColor = std::make_unique<Color>(0.1f, 0.25f, 0.5f, 1.0f);
}

void MultiRenderTarget::initialize(std::uint32_t size) {
	initialize(WinApp::GetClientWidth(), WinApp::GetClientHight(), size);
}

std::vector<OffscreenRender>& MultiRenderTarget::offscreen_render_list() {
	return renderTargets;
}

const std::vector<OffscreenRender>& MultiRenderTarget::offscreen_render_list() const {
	return renderTargets;
}

void MultiRenderTarget::set_render_target() {
	auto&& commandList = DirectXCommand::GetCommandList();
	commandList->OMSetRenderTargets(
		static_cast<UINT>(renderTargetsHandles.size()), renderTargetsHandles.data(),
		depthStencil ? 1 : 0,
		depthStencil ? &depthStencil->get_dsv_cpu_handle() : nullptr
	);
	for (auto itr = renderTargets.begin(); itr < renderTargets.end(); ++itr) {
		commandList->ClearRenderTargetView(
			itr->get_cpu_handle(),
			&clearColor->red, 0, nullptr
		);
	}
}

void MultiRenderTarget::change_render_target_state() {
	for (auto itr = renderTargets.begin(); itr < renderTargets.end(); ++itr) {
		itr->change_resource_state();
	}
}
