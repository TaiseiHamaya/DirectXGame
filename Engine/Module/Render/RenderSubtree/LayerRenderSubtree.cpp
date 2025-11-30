#include "LayerRenderSubtree.h"

#include "Engine/Module/Render/RenderPSO/Deferred/Lighting/DirectionalLighingNode.h"
#include "Engine/Module/Render/RenderPSO/Deferred/Lighting/NonLightingPixelNode.h"
#include "Engine/Module/Render/RenderPSO/Deferred/Lighting/PointLightingNode.h"
#include "Engine/Module/Render/RenderPSO/Deferred/Mesh/SkinningMeshNodeDeferred.h"
#include "Engine/Module/Render/RenderPSO/Deferred/Mesh/StaticMeshNodeDeferred.h"
#include "Engine/Module/Render/RenderPSO/Forward/Primitive/Rect3dNode.h"
#include "Engine/Module/Render/RenderPSO/Forward/FontRenderingNode/FontRenderingNode.h"

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
			nonLightingPixelNode = std::make_shared<NonLightingPixelNode>();
			nonLightingPixelNode->initialize();
			nonLightingPixelNode->set_gbuffers(gBuffer[0]);
			nodes.emplace_back(nonLightingPixelNode);
		}
		{
			directionalLightingNode = std::make_shared<DirectionalLightingNode>();
			directionalLightingNode->initialize();
			directionalLightingNode->set_gbuffers(gBuffer);
			nodes.emplace_back(directionalLightingNode);
		}
		{
			pointLightingNode = std::make_shared<PointLightingNode>();
			pointLightingNode->initialize();
			pointLightingNode->set_gbuffers(gBuffer);
			nodes.emplace_back(pointLightingNode);
		}
	}
	// Primitiveパス
	{
		auto node = std::make_shared<Rect3dNode>();
		node->initialize();
		nodes.emplace_back(node);
	}
	{
		auto node = std::make_shared<FontRenderingNode>();
		node->initialize();
		nodes.emplace_back(node);
	}

	renderSubtree.initialize(std::move(nodes));
}

void LayerRenderSubtree::begin_nodes() {
	renderSubtree.begin();
	counter = 1;
}

void LayerRenderSubtree::next_node() {
	renderSubtree.next();
	switch (counter) {
	case 2:
		nonLightingPixelNode->stack_lighting_command();
		break;
	case 3:
		directionalLightingNode->stack_lighting_command();
		break;
	case 4:
		pointLightingNode->stack_lighting_command();
		break;
	default:
		break;
	}
	++counter;
}
