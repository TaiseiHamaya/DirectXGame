#include "MultiRenderTarget.h"

//#include <d3d12.h>
//
//#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
//#include "Engine/DirectX/DxResource/DepthStencil/DepthStencil.h"
//#include "Engine/DirectX/DxResource/OffscreenRender/OffscreenRender.h"
//
//MultiRenderTarget::MultiRenderTarget() = default;
//
//MultiRenderTarget::~MultiRenderTarget() noexcept = default;
//
//void MultiRenderTarget::initialize() {
//	initialize(WinApp::GetClientWidth(), WinApp::GetClientHight(), 1);
//}
//
//void MultiRenderTarget::finalize() {
//	for (auto& renderTarget : renderTargets) {
//		renderTarget.release_index();
//	}
//}
//
//void MultiRenderTarget::initialize(std::uint32_t width, std::uint32_t hight, std::uint32_t size) {
//	renderTargets.resize(size);
//	for (auto itr = renderTargets.begin(); itr < renderTargets.end(); ++itr) {
//		itr->initialize(width, hight);
//		renderTargetsHandles.emplace_back(itr->get_cpu_handle());
//	}
//	create_view_port(width, hight);
//}
//
//void MultiRenderTarget::initialize(std::uint32_t size) {
//	initialize(WinApp::GetClientWidth(), WinApp::GetClientHight(), size);
//}
//
//std::vector<OffscreenRender>& MultiRenderTarget::offscreen_render_list() {
//	return renderTargets;
//}
//
//const std::vector<OffscreenRender>& MultiRenderTarget::offscreen_render_list() const {
//	return renderTargets;
//}
//
//void MultiRenderTarget::set_render_target(const std::shared_ptr<DepthStencil>& depthStencil) {
//	auto&& commandList = DxCommand::GetCommandList();
//	commandList->OMSetRenderTargets(
//		static_cast<UINT>(renderTargetsHandles.size()), renderTargetsHandles.data(),
//		depthStencil ? 1 : 0,
//		depthStencil ? &depthStencil->get_dsv_cpu_handle() : nullptr
//	);
//}
//
//void MultiRenderTarget::clear_render_target() {
//	auto&& commandList = DxCommand::GetCommandList();
//	for (OffscreenRender& renderTarget : renderTargets) {
//		renderTarget.clear_resource();
//	}
//}
//
//void MultiRenderTarget::change_render_target_state() {
//	for (OffscreenRender& renderTarget : renderTargets) {
//		renderTarget.change_resource_state();
//	}
//}
