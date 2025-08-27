#include "RenderDAG.h"

#include <format>

#include "Engine/Assets/Json/JsonAsset.h"
#include "Engine/Module/Render/RenderNode/IRenderNode.h"
#include "Engine/Module/Render/RenderNode/Loader/RenderNodeLoader.h"
#include "Engine/Runtime/Scene/Scene.h"

void RenderDAG::setup(const std::string& sceneName, Reference<Scene> scene) {
	JsonAsset json{ std::format("./Game/Core/Scene/{}/RenderDAG.json", sceneName) };

	if (json.cget().contains("RenderTextures")) {
		renderTargetCollection.setup(json.cget()["RenderTextures"]);
	}

	RenderNodeLoader loader;
	loader.setup(scene, renderTargetCollection);

	if (json.cget().contains("RenderNodes")) {
		renderGraph = loader.entry_point(json.cget()["RenderNodes"]);
	}
}

void RenderDAG::render_entry_point() const {
	for (const std::unique_ptr<IRenderNode>& node : renderGraph) {
		if (node) {
			node->stack_command();
		}
	}
}
