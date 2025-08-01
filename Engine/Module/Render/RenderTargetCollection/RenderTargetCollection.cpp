#include "RenderTargetCollection.h"

#include "Engine/Application/Output.h"
#include "Engine/Module/Render/RenderTargetGroup/SingleRenderTarget.h"

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
	if (index >= renderTargets.size()) {
		Warning("Try to reference render target out of range index-\'{}\'.", index);
		return nullptr;
	}
	return renderTargets[index];
}
