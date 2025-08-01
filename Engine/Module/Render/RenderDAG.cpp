#include "RenderDAG.h"

#include <format>

#include "Engine/Assets/Json/JsonAsset.h"
#include "Engine/Module/Render/RenderNode/IRenderNode.h"

void RenderDAG::setup(const std::string& sceneName) {
	JsonAsset json{ std::format("./Game/Core/Scene/{}/RenderDAG.json", sceneName) };

	if (json.cget().contains("RenderTextures")) {
		renderTargetCollection.setup(json.cget()["RenderTextures"]);
	}
	if (json.cget().contains("RenderNodes")) {
		load_render_graph(json.cget()["RenderNodes"]);
	}
}

void RenderDAG::render_entry_point() const {
	for (const std::unique_ptr<IRenderNode>& node : renderGraph) {
		if (node) {
			node->stack_command();
		}
	}
}

void RenderDAG::load_render_graph(const nlohmann::json& json) {
	for (auto& [_, value] : json.items()) {
		auto& node = renderGraph.emplace_back();
		//node = RenderNodeSerializeer::();
	}

}
