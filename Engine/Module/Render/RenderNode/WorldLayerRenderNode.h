#pragma once

#include "./IRenderNode.h"

#include <Library/Utility/Template/Reference.h>

#include "Engine/Module/Render/RenderPSO/Deferred/DeferredAdaptor.h"
#include "Engine/Module/Render/RenderSubtree/LayerRenderSubtree.h"
#include "Engine/Module/Render/RenderTargetGroup/BaseRenderTargetGroup.h"

class Camera3D;
class WorldRenderCollection;

class WorldLayerRenderNode final : public IRenderNode {
private:
	struct GBufferData {
		std::array<RenderTexture, DeferredAdaptor::NUM_GBUFFER> texture;
		DeferredAdaptor::GBuffersType renderTarget;
		RECT rect;
		D3D12_VIEWPORT viewport;
	};

public:
	WorldLayerRenderNode() = default;
	~WorldLayerRenderNode() noexcept = default;

	__CLASS_NON_COPYABLE(WorldLayerRenderNode)

public:
	void initialize();

	void setup(Reference<WorldRenderCollection> worldRenderCollection);

	void stack_command() override;

private:
	Reference<BaseRenderTargetGroup> outputRenderTargetGroup;
	GBufferData gBuffer;
	Reference<WorldRenderCollection> worldRenderCollection;
	u8 renderLayer;
	Reference<Camera3D> camera;

	RECT rect;
	D3D12_VIEWPORT viewport;
	LayerRenderSubtree subtree;
};
