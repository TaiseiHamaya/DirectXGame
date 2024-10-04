#include "SingleRenderTargetNode.h"

#include <cassert>

#include "Engine/Render/RenderTargetGroup/SingleRenderTarget.h"
#include "Engine/Render/RenderTargetGroup/SwapChainRenderTargetGroup.h"
#include "Engine/DirectX/DirectXResourceObject/OffscreenRender/OffscreenRender.h"

SingleRenderTargetNode::SingleRenderTargetNode() = default;

SingleRenderTargetNode::~SingleRenderTargetNode() noexcept = default;

void SingleRenderTargetNode::set_render_target(const std::shared_ptr<SingleRenderTarget>& renderTarget_) {
	if (renderTarget_) {
		renderTarget = renderTarget_;
		resultSvtHandle = renderTarget_->main_offscreen_render().texture_gpu_handle();
	}
	else {
		auto temp = std::make_shared<SingleRenderTarget>();
		renderTarget = temp;
		renderTarget->initialize();
		resultSvtHandle = temp->main_offscreen_render().texture_gpu_handle();
	}
}

void SingleRenderTargetNode::set_render_target_SC(const std::shared_ptr<SwapChainRenderTargetGroup>& renderTarget_) {
	if (renderTarget_) {
		renderTarget = renderTarget_;
		config |= RenderNodeConfig::ContinueDrawBefore;
	}
}

const D3D12_GPU_DESCRIPTOR_HANDLE& SingleRenderTargetNode::result_stv_handle() const {
	assert(resultSvtHandle.has_value());
	return resultSvtHandle.value();
}
