#include "RenderDAG.h"

#include <format>

#include "Engine/Assets/Json/JsonAsset.h"
#include "Engine/Module/Render/RenderNode/IRenderNode.h"

void RenderDAG::setup(std::filesystem::path sceneName) {
	JsonAsset json{ std::format("./Game/Core/{}/RenderDAG.json", sceneName.string()) };

	
}

void RenderDAG::render_entry_point() const {
	for(const std::unique_ptr<IRenderNode>& node : renderGraph) {
		if(node) {
			node->stack_command();
		}
	}
}
