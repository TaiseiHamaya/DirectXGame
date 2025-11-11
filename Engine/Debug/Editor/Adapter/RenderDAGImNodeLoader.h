#pragma once

#include <memory>

#include <ImNodeFlow/include/ImNodeFlow.h>

#include <Library/Utility/Template/Reference.h>

#include "../Window/RenderDagImNode/PostEffectImNode.h"
#include "../Window/RenderDagImNode/StaticTextureImNode.h"
#include "../Window/RenderDagImNode/WorldLayerRenderImNode.h"

/// <summary>
/// RenderDAGmNodeのロード用関数オブジェクト
/// </summary>
/// <typeparam name="T"></typeparam>
template<typename T>
class RenderDAGLoader {
public:
	std::shared_ptr<PostEffectImNode> operator()(u64 outputId, Reference<ImFlow::ImNodeFlow> imNodeFlow, const nlohmann::json& json) {
		auto result = imNodeFlow->addNode<PostEffectImNode>({ 0,0 }, outputId);
		PostEffectImNode::Data data;
		json.get_to(data.outputSize);
		json.get_to(data.peType);
		result->set_data(data);
		return result;
	};

	std::shared_ptr<StaticTextureImNode> operator()(u64 outputId, Reference<ImFlow::ImNodeFlow> imNodeFlow, const nlohmann::json& json) {
		auto result = imNodeFlow->addNode<StaticTextureImNode>({ 0,0 }, outputId);
		StaticTextureImNode::Data data;
		json.get_to(data.textureFilename);
		result->set_data(data);
		return result;
	};

	std::shared_ptr<WorldLayerRenderImNode> operator()(u64 outputId, Reference<ImFlow::ImNodeFlow> imNodeFlow, const nlohmann::json& json) {
		auto result = imNodeFlow->addNode<WorldLayerRenderImNode>({ 0,0 }, outputId);
		WorldLayerRenderImNode::Data data;
		json.get_to(data.outputSize);
		json.get_to(data.worldIndex);
		json.get_to(data.layerIndex);
		json.get_to(data.cameraId);
		json.get_to(data.gBufferSize);
		json.get_to(data.gBufferViewport.left);
		json.get_to(data.gBufferViewport.top);
		json.get_to(data.gBufferViewport.width);
		json.get_to(data.gBufferViewport.height);
		json.get_to(data.gBufferViewport.minDepth);
		json.get_to(data.gBufferViewport.maxDepth);
		json.get_to(data.layerScissor.topLeft);
		json.get_to(data.layerScissor.bottomRight);
		result->set_data(data);
		return result;
	};
};
