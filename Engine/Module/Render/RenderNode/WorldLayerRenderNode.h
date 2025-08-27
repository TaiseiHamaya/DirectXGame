#pragma once

#include "./IRenderNode.h"

#include <array>
#include <utility>

#include <Library/Utility/Template/Reference.h>

#include "Engine/Module/Render/RenderPSO/Deferred/DeferredAdaptor.h"
#include "Engine/Module/Render/RenderSubtree/LayerRenderSubtree.h"
#include "Engine/Module/Render/RenderTargetGroup/BaseRenderTargetGroup.h"

class Camera3D;
class WorldRenderCollection;

class WorldLayerRenderNode final : public IRenderNode {
public:
	struct GBufferData {
		std::array<RenderTexture, DeferredAdaptor::NUM_GBUFFER> texture;
		DeferredAdaptor::GBuffersType renderTarget;
		RECT rect;
		D3D12_VIEWPORT viewport;
	};

	struct LayerData {
		Reference<WorldRenderCollection> worldRenderCollection;
		u8 index;
		Reference<Camera3D> camera;

		RECT rect;
		D3D12_VIEWPORT viewport;
	};

	struct Data {
		GBufferData gBuffer;

		LayerData layerData;

		Reference<BaseRenderTargetGroup> outputRenderTargetGroup;
	};

public:
	WorldLayerRenderNode() = default;
	~WorldLayerRenderNode() noexcept = default;

	__CLASS_NON_COPYABLE(WorldLayerRenderNode)

public:
	void initialize();

	void stack_command() override;

public:
	void set_data(Data&& data_) { data = std::move(data_); };
	const Data& get_data() const { return data; };

private:
	LayerRenderSubtree subtree;

	Data data;
};
