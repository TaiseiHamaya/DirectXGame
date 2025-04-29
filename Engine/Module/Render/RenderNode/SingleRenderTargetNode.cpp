#include "SingleRenderTargetNode.h"

#include "Engine/GraphicsAPI/DirectX/DxSwapChain/DxSwapChain.h"
#include "Engine/Module/Render/RenderTargetGroup/SingleRenderTarget.h"
#include "Engine/Module/Render/RenderTargetGroup/SwapChainRenderTargetGroup.h"

SingleRenderTargetNode::SingleRenderTargetNode() = default;

SingleRenderTargetNode::~SingleRenderTargetNode() noexcept = default;

void SingleRenderTargetNode::set_render_target(Reference<SingleRenderTarget> renderTargetGroup) {
	renderTarget = renderTargetGroup;
}

void SingleRenderTargetNode::set_render_target_SC() {
	renderTarget = DxSwapChain::GetRenderTarget();
}
