#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <json.hpp>

#include <filesystem>
#include <unordered_map>

#include "../Window/RenderDagImNode/PostEffectImNode.h"
#include "../Window/RenderDagImNode/ResultImNode.h"
#include "../Window/RenderDagImNode/StaticTextureImNode.h"
#include "../Window/RenderDagImNode/WorldLayerRenderImNode.h"
#include "Engine/Debug/Editor/Window/EditorRenderDAG.h"

/// <summary>
/// RenderPath保存用関数オブジェクト
/// </summary>
namespace szg {

class RenderDAGImNodeSaver {
public:
	void entry_point(const std::filesystem::path& filePath, const std::unordered_map<u64, EditorRenderDAG::DAGNodeType>& nodes);

	nlohmann::json operator()(const std::shared_ptr<WorldLayerRenderImNode>& node);

	nlohmann::json operator()(const std::shared_ptr<PostEffectImNode>& node);

	nlohmann::json operator()(const std::shared_ptr<StaticTextureImNode>& node);

	nlohmann::json operator()(const std::shared_ptr<ResultImNode>&);
};

}; // szg

#endif // DEBUG_FEATURES_ENABLE
