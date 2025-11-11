#pragma once

#include <json.hpp>

#include "../Window/RenderDagImNode/PostEffectImNode.h"
#include "../Window/RenderDagImNode/ResultImNode.h"
#include "../Window/RenderDagImNode/StaticTextureImNode.h"
#include "../Window/RenderDagImNode/WorldLayerRenderImNode.h"

class RenderDAGImNodeSaver {
public:
	nlohmann::json operator()(const std::shared_ptr<WorldLayerRenderImNode>& node) {
		if (!node) {
			return {};
		}
		nlohmann::json json;
		json["Type"] = 0;
		const WorldLayerRenderImNode::Data& data = node->get_data();
		json["Data"].update(data.outputSize);
		json["Data"].update(data.worldIndex);
		json["Data"].update(data.layerIndex);
		json["Data"].update(data.cameraId);
		json["Data"].update(data.gBufferSize);
		json["Data"].update(data.gBufferViewport.left);
		json["Data"].update(data.gBufferViewport.top);
		json["Data"].update(data.gBufferViewport.width);
		json["Data"].update(data.gBufferViewport.height);
		json["Data"].update(data.gBufferViewport.minDepth);
		json["Data"].update(data.gBufferViewport.maxDepth);
		json["Data"].update(data.layerScissor.topLeft);
		json["Data"].update(data.layerScissor.bottomRight);
		return json;
	}

	nlohmann::json operator()(const std::shared_ptr<PostEffectImNode>& node) {
		if (!node) {
			return {};
		}
		nlohmann::json json;
		json["Type"] = 1;
		const PostEffectImNode::Data& data = node->get_data();
		json["Data"].update(data.outputSize);
		json["Data"].update(data.peType);
		return json;
	}

	nlohmann::json operator()(const std::shared_ptr<StaticTextureImNode>& node) {
		if (!node) {
			return {};
		}
		nlohmann::json json;
		json["Type"] = 2;
		const StaticTextureImNode::Data& data = node->get_data();
		json["Data"].update(data.textureFilename);
		return json;
	}

	nlohmann::json operator()(const std::shared_ptr<ResultImNode>&) {
		return {};
	}
};
