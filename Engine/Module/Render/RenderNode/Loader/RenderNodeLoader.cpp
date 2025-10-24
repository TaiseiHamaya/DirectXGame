#include "RenderNodeLoader.h"

#include "Engine/Module/Render/RenderTargetCollection/RenderTargetCollection.h"
#include "Engine/Runtime/Scene/Scene.h"

#include "Engine/Module/Render/RenderNode/PostEffectNode.h"
#include "Engine/Module/Render/RenderNode/WorldLayerRenderNode.h"

void RenderNodeLoader::setup(Reference<Scene> scene_, Reference<RenderTargetCollection> renderTargetCollection_) {
	scene = scene_;
	renderTargetCollection = renderTargetCollection_;
}

std::vector<std::unique_ptr<IRenderNode>> RenderNodeLoader::entry_point(const nlohmann::json& json) {
	std::vector<std::unique_ptr<IRenderNode>> result;

	for (auto& [_, value] : json.items()) {
		if (!value.contains("Type")) {
			szgWarning("RenderNodeLoader::entry_point: Node Type is not specified.");
			continue;
		}
		const std::string type = value["Type"].get<std::string>();
		if (type == "PostEffect") {
			result.emplace_back(load_post_effect_node(value));
		}
		else if (type == "WorldLayer") {
			result.emplace_back(load_world_lender_layer_node(value));
		}
		else {
			szgWarning("RenderNodeLoader::entry_point: Unknown Node Type \'{}\'", type);
			continue;
		}
	}

	return result;
}

std::unique_ptr<IRenderNode> RenderNodeLoader::load_post_effect_node(const nlohmann::json& json) {
	std::unique_ptr<PostEffectNode> result = std::make_unique<PostEffectNode>();
	result->initialize();

	PostEffectNode::Data resultData{};
	{ // output
		const nlohmann::json& outputJson = json["Output"];
		const u32 renderTargetIndex = outputJson["RenderTarget"].get<u32>();
		resultData.outRenderTargetGroup = renderTargetCollection->get_render_target(renderTargetIndex);
	}
	{ // rect & viewport
		const nlohmann::json& rectJson = json["Rect"];
		resultData.rect = {
			rectJson["Left"].get<LONG>(),
			rectJson["Top"].get<LONG>(),
			rectJson["Right"].get<LONG>(),
			rectJson["Bottom"].get<LONG>()
		};
		const nlohmann::json& viewportJson = json["Viewport"];
		resultData.viewport = {
			viewportJson["Left"].get<FLOAT>(),
			viewportJson["Top"].get<FLOAT>(),
			viewportJson["Width"].get<FLOAT>(),
			viewportJson["Height"].get<FLOAT>(),
			viewportJson["MinDepth"].get<FLOAT>(),
			viewportJson["MaxDepth"].get<FLOAT>()
		};
	}
	{ // PostEffectPSO
		const nlohmann::json& psoJson = json["PSO"];

		resultData.postEffectPSO = pePsoLoader.entry_point(psoJson);
	}

	result->set_data(std::move(resultData));

	return result;
}

std::unique_ptr<IRenderNode> RenderNodeLoader::load_world_lender_layer_node(const nlohmann::json& json) {
	std::unique_ptr<WorldLayerRenderNode> result = std::make_unique<WorldLayerRenderNode>();
	WorldLayerRenderNode::Data resultData{};

	result->initialize();

	{ // GBuffer
		const nlohmann::json& gBufferJson = json["GBuffer"];
		resultData.gBuffer.rect = {
			gBufferJson["Rect"]["Left"].get<LONG>(),
			gBufferJson["Rect"]["Top"].get<LONG>(),
			gBufferJson["Rect"]["Right"].get<LONG>(),
			gBufferJson["Rect"]["Bottom"].get<LONG>()
		};
		resultData.gBuffer.viewport = {
			gBufferJson["Viewport"]["Left"].get<FLOAT>(),
			gBufferJson["Viewport"]["Top"].get<FLOAT>(),
			gBufferJson["Viewport"]["Width"].get<FLOAT>(),
			gBufferJson["Viewport"]["Height"].get<FLOAT>(),
			gBufferJson["Viewport"]["MinDepth"].get<FLOAT>(),
			gBufferJson["Viewport"]["MaxDepth"].get<FLOAT>()
		};
	}

	{ // Layer
		const nlohmann::json& jsonLayer = json["Layer"];
		resultData.layerData.rect = {
			jsonLayer["Rect"]["Left"].get<LONG>(),
			jsonLayer["Rect"]["Top"].get<LONG>(),
			jsonLayer["Rect"]["Right"].get<LONG>(),
			jsonLayer["Rect"]["Bottom"].get<LONG>()
		};
		resultData.layerData.viewport = {
			jsonLayer["Viewport"]["Left"].get<FLOAT>(),
			jsonLayer["Viewport"]["Top"].get<FLOAT>(),
			jsonLayer["Viewport"]["Width"].get<FLOAT>(),
			jsonLayer["Viewport"]["Height"].get<FLOAT>(),
			jsonLayer["Viewport"]["MinDepth"].get<FLOAT>(),
			jsonLayer["Viewport"]["MaxDepth"].get<FLOAT>()
		};
		const u32 worldIndex = json["Layer"]["World"].get<u32>();
		resultData.layerData.index = json["Layer"]["Layer"].get<u8>();
		resultData.layerData.worldRenderCollection = scene->get_world(worldIndex)->render_collection();

		resultData.layerData.camera = nullptr;
	}

	{ // output
		const nlohmann::json& outputJson = json["Output"];
		const u32 renderTargetIndex = outputJson["RenderTarget"].get<u32>();
		resultData.outputRenderTargetGroup = renderTargetCollection->get_render_target(renderTargetIndex);
	}

	result->set_data(std::move(resultData));

	return result;
}
