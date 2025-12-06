#include "LayerRenderSubtree.h"

using namespace szg;

#include "Engine/Module/Render/RenderPipeline/Deferred/Lighting/DirectionalLightingPipeline.h"
#include "Engine/Module/Render/RenderPipeline/Deferred/Lighting/NonLightingPixelPipeline.h"
#include "Engine/Module/Render/RenderPipeline/Deferred/Lighting/PointLightingPipeline.h"
#include "Engine/Module/Render/RenderPipeline/Deferred/Mesh/SkinningMeshDeferredPipeline.h"
#include "Engine/Module/Render/RenderPipeline/Deferred/Mesh/StaticMeshDeferredPipeline.h"
#include "Engine/Module/Render/RenderPipeline/Forward/FontRenderingNode/FontRenderingPipeline.h"
#include "Engine/Module/Render/RenderPipeline/Forward/Primitive/Rect3dPipeline.h"

void LayerRenderSubtree::setup(std::array<Reference<RenderTexture>, DeferredAdaptor::NUM_GBUFFER> gBuffer) {
	std::vector<std::shared_ptr<BaseRenderPipeline>> nodes;
	//// WriteGBuffer
	{
		auto node = std::make_shared<StaticMeshDeferredPipeline>();
		node->initialize();
		nodes.emplace_back(node);
	}
	{
		auto node = std::make_shared<SkinningMeshDeferredPipeline>();
		node->initialize();
		nodes.emplace_back(node);
	}
	// ライティングパス
	{
		{
			nonLightingPixelNode = std::make_shared<NonLightingPixelPipeline>();
			nonLightingPixelNode->initialize();
			nonLightingPixelNode->set_gbuffers(gBuffer[0]);
			nodes.emplace_back(nonLightingPixelNode);
		}
		{
			directionalLightingNode = std::make_shared<DirectionalLightingPipeline>();
			directionalLightingNode->initialize();
			directionalLightingNode->set_gbuffers(gBuffer);
			nodes.emplace_back(directionalLightingNode);
		}
		{
			pointLightingNode = std::make_shared<PointLightingPipeline>();
			pointLightingNode->initialize();
			pointLightingNode->set_gbuffers(gBuffer);
			nodes.emplace_back(pointLightingNode);
		}
	}
	// Primitiveパス
	{
		auto node = std::make_shared<Rect3dPipeline>();
		node->initialize();
		nodes.emplace_back(node);
	}
	{
		auto node = std::make_shared<FontRenderingPipeline>();
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
