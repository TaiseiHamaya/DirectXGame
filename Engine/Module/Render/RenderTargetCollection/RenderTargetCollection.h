#pragma once

#include <array>
#include <utility>

#include <json.hpp>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h"
#include "Engine/Module/Render/RenderPipeline/Deferred/DeferredAdaptor.h"
#include "Engine/Module/Render/RenderTargetGroup/BaseRenderTargetGroup.h"

namespace szg {

class RenderTargetCollection {
public:
	RenderTargetCollection() = default;
	~RenderTargetCollection() noexcept = default;

	SZG_CLASS_MOVE_ONLY(RenderTargetCollection)

public:
	std::pair<Reference<BaseRenderTargetGroup>, Reference<RenderTexture>> create_render_target_group(const nlohmann::json& json);
	std::pair<Reference<DeferredAdaptor::GBuffersType>, std::array<Reference<RenderTexture>, DeferredAdaptor::NUM_GBUFFER>> create_gbuffer(const nlohmann::json& json);

	void clear();

private:
	std::vector<std::unique_ptr<BaseRenderTargetGroup>> renderTargets;
	std::vector<std::unique_ptr<RenderTexture>> renderTextures;
};

}; // szg
