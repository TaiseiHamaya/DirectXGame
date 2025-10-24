#include "RenderTargetCollection.h"

#include "Engine/Application/Logger.h"
#include "Engine/Module/Render/RenderTargetGroup/SingleRenderTarget.h"
#include "Engine/Module/Render/RenderTargetGroup/SwapChainRenderTargetGroup.h"
#include "Engine/GraphicsAPI/DirectX/DxSwapChain/DxSwapChain.h"

void RenderTargetCollection::setup(const nlohmann::json& json) {
	for (auto& [_, value] : json.items()) {
		auto& texture = renderTextures.emplace_back();
		texture.initialize(
			value["Format"].get<DXGI_FORMAT>(),
			value["Width"].get<u32>(),
			value["Height"].get<u32>()
		);
		std::unique_ptr<SingleRenderTarget> renderTarget = std::make_unique<SingleRenderTarget>();
		renderTarget->initialize(texture);
		renderTargets.emplace_back(std::move(renderTarget));
	}
}

Reference<BaseRenderTargetGroup> RenderTargetCollection::get_render_target(u32 index) const {
	// 末尾指定の場合、Screenを指定
	if (index == renderTargets.size()) {
		return DxSwapChain::GetRenderTarget();
	}
	else if (index > renderTargets.size()) {
		szgWarning("Try to reference render target out of range index-\'{}\'.", index);
		return nullptr;
	}
	return renderTargets[index];
}
