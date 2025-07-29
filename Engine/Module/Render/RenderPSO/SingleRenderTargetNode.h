#pragma once

#include "Engine/Module/Render/RenderPSO/BaseRenderNode.h"

class SingleRenderTarget;
class SwapChainRenderTargetGroup;

class SingleRenderTargetNode : public BaseRenderNode {
public:
	SingleRenderTargetNode();
	virtual ~SingleRenderTargetNode() noexcept;

	SingleRenderTargetNode(const SingleRenderTargetNode&) = delete;
	SingleRenderTargetNode& operator=(const SingleRenderTargetNode&) = delete;
	SingleRenderTargetNode(SingleRenderTargetNode&&) = default;
	SingleRenderTargetNode& operator=(SingleRenderTargetNode&&) = default;
};
