#pragma once

#include <json.hpp>

#include <Library/Utility/Tools/ConstructorMacro.h>
#include <Library/Utility/Template/Reference.h>

#include "Engine/Module/Render/RenderTargetGroup/BaseRenderTargetGroup.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h"

class RenderTargetCollection {
public:
	RenderTargetCollection() = default;
	~RenderTargetCollection() noexcept = default;

	__CLASS_NON_COPYABLE(RenderTargetCollection)

public:
	void setup(const nlohmann::json& json);

	Reference<BaseRenderTargetGroup> get_render_target(u32 index) const;

private:
	std::vector<std::unique_ptr<BaseRenderTargetGroup>> renderTargets;
	std::vector<RenderTexture> renderTextures;
};
