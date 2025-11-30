#include "RenderTargetCollection.h"

#include "Engine/Module/Render/RenderPSO/Deferred/DeferredAdaptor.h"
#include "Engine/Module/Render/RenderTargetGroup/SingleRenderTarget.h"

std::pair<Reference<BaseRenderTargetGroup>, Reference<RenderTexture>> RenderTargetCollection::create_render_target_group(const nlohmann::json& json) {
	std::unique_ptr<SingleRenderTarget> result;

	auto& texture = renderTextures.emplace_back();
	texture = std::make_unique<RenderTexture>();
	texture->initialize(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		json.value("OutputSize", nlohmann::json::object()).value("X", ProjectSettings::ClientWidth()),
		json.value("OutputSize", nlohmann::json::object()).value("Y", ProjectSettings::ClientHeight())
	);
	result = std::make_unique<SingleRenderTarget>();
	result->initialize(texture);

	renderTargets.emplace_back(std::move(result));
	return std::make_pair<Reference<BaseRenderTargetGroup>, Reference<RenderTexture>>(renderTargets.back(), texture);
}

std::pair<Reference<DeferredAdaptor::GBuffersType>, std::array<Reference<RenderTexture>, DeferredAdaptor::NUM_GBUFFER>> RenderTargetCollection::create_gbuffer(const nlohmann::json& json) {
	std::unique_ptr<DeferredAdaptor::GBuffersType> result = std::make_unique<DeferredAdaptor::GBuffersType>();
	Reference<DeferredAdaptor::GBuffersType> temp = result;
	std::array<Reference<RenderTexture>, DeferredAdaptor::NUM_GBUFFER> gbufferTextures;

	for (i32 i = 0; i < DeferredAdaptor::NUM_GBUFFER; ++i) {
		gbufferTextures[i] = renderTextures.emplace_back(std::make_unique<RenderTexture>());
		gbufferTextures[i]->initialize(
			DeferredAdaptor::DXGI_FORMAT_LIST[i],
			json.value("GBufferSize", nlohmann::json::object()).value("X", ProjectSettings::ClientWidth()),
			json.value("GBufferSize", nlohmann::json::object()).value("Y", ProjectSettings::ClientHeight())
		);
	}
	result->initialize(gbufferTextures);
	renderTargets.emplace_back(std::move(result));
	return std::make_pair(temp, gbufferTextures);
}
