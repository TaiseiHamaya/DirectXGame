#include "RenderDAG.h"

#include <format>

#include "Engine/Assets/Json/JsonAsset.h"

void RenderDAG::setup(std::filesystem::path sceneName) {
	JsonAsset json{ std::format("./Game/Core/{}/RenderDAG.json", sceneName.string()) };

	
}

void RenderDAG::render_entry_point() {
	for(std::unique_ptr<IRenderVertex>& vertex : renderGraph) {
		if(vertex) {
			//vertex->render();
		}
	}
}
