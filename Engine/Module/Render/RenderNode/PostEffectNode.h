#pragma once

#include <d3d12.h>

#include "./IRenderNode.h"

#include <Library/Utility/Template/Reference.h>

#include <Engine/Module/Render/RenderPSO/PostEffectPSO.h>
#include <Engine/Module/Render/RenderTargetGroup/BaseRenderTargetGroup.h>

class PostEffectNode final : public IRenderNode {

public:
	PostEffectNode() = default;
	~PostEffectNode() noexcept = default;

	__CLASS_NON_COPYABLE(PostEffectNode)

public:
	void stack_command() override;

private:
	Reference<BaseRenderTargetGroup> outRenderTargetGroup;

	RECT rect;
	D3D12_VIEWPORT viewport;
	
	std::unique_ptr<PostEffectPSO> postEffectPSO;
};
