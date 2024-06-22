#pragma once

#include "Engine/Render/RenderNode/BaseRenderNode.h"

#include <array>
#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"

class RenderTarget;
class PipelineState;

class SwapChainRenderNode : public BaseRenderNode {
public:
	SwapChainRenderNode() = default;
	~SwapChainRenderNode() = default;

public:
	void initialize() override;
	void begin() override;

public:
	void set_pso(std::unique_ptr<PipelineState>&& piplineState_);
	void set_render_target(const std::array<RenderTarget, SWAPCHAIN_HEAP>& renderTargets);
	void set_clear_color(const Color& color_);
	DepthStencil& get_depth_stencil();
};
