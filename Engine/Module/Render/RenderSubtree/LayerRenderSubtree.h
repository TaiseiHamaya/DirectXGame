#pragma once

#include <array>

#include <Library/Utility/Template/Reference.h>

#include "../RenderPath/RenderPath.h"
#include "Engine/Module/Render/RenderPSO/Deferred/DeferredAdaptor.h"

class RenderTexture;

class LayerRenderSubtree {
public:
	void setup(std::array<Reference<RenderTexture>, DeferredAdaptor::NUM_GBUFFER> gBuffer);

	void begin_nodes();
	void next_node();

private:
	RenderPath renderSubtree;
};
