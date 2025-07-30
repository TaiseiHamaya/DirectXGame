#include "LayerRenderSubtree.h"

#include "Engine/Assets/Json/JsonAsset.h"
#include "Engine/Module/Render/RenderPSO/Deferred/Lighting/DirectionalLighingNode.h"
#include "Engine/Module/Render/RenderPSO/Deferred/Lighting/NonLightingPixelNode.h"
#include "Engine/Module/Render/RenderPSO/Deferred/Lighting/PointLightingNode.h"
#include "Engine/Module/Render/RenderPSO/Deferred/Mesh/SkinningMeshNodeDeferred.h"
#include "Engine/Module/Render/RenderPSO/Deferred/Mesh/StaticMeshNodeDeferred.h"
#include "Engine/Module/Render/RenderPSO/Forward/Primitive/Rect3dNode.h"

void LayerRenderSubtree::setup(std::array<Reference<RenderTexture>, DeferredAdaptor::NUM_GBUFFER> gBuffer) {
	std::vector<std::shared_ptr<BaseRenderNode>> nodes;
	//// WriteGBuffer
	{
		auto node = std::make_shared<StaticMeshNodeDeferred>();
		node->initialize();
		nodes.emplace_back(node);
	}
	{
		auto node = std::make_shared<SkinningMeshNodeDeferred>();
		node->initialize();
		nodes.emplace_back(node);
	}
	// ライティングパス
	{
		{
			auto node = std::make_shared<NonLightingPixelNode>();
			node->initialize();
			node->set_gbuffers(gBuffer[0]);
			nodes.emplace_back(node);
		}
		{
			auto node = std::make_shared<DirectionalLightingNode>();
			node->initialize();
			node->set_gbuffers(gBuffer);
			nodes.emplace_back(node);
		}
		{
			auto node = std::make_shared<PointLightingNode>();
			node->initialize();
			node->set_gbuffers(gBuffer);
			nodes.emplace_back(node);
		}
	}
	// Primitiveパス
	{
		auto node = std::make_shared<Rect3dNode>();
		node->initialize();
		nodes.emplace_back(node);
	}

	renderSubtree.initialize(std::move(nodes));
}

void LayerRenderSubtree::begin_nodes() {
	renderSubtree.begin();
}

void LayerRenderSubtree::next_node() {
	renderSubtree.next();
}
