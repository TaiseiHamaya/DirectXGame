#pragma once

#include <array>

#include <Library/Utility/Template/Reference.h>

#include "../RenderPath/RenderPath.h"
#include "Engine/Module/Render/RenderPipeline/Deferred/DeferredAdaptor.h"

class RenderTexture;
class NonLightingPixelPipeline;
class DirectionalLightingPipeline;
class PointLightingPipeline;

class LayerRenderSubtree {
public:
	void setup(std::array<Reference<RenderTexture>, DeferredAdaptor::NUM_GBUFFER> gBuffer);

	void begin_nodes();
	void next_node();

private:
	RenderPath renderSubtree;

	i32 counter{0};

	std::shared_ptr<NonLightingPixelPipeline> nonLightingPixelNode;
	std::shared_ptr<DirectionalLightingPipeline> directionalLightingNode;
	std::shared_ptr<PointLightingPipeline> pointLightingNode;
};
