#include "RenderDAG.h"

#include <format>

#include "Engine/Assets/Json/JsonAsset.h"
#include "Engine/Module/Render/RenderNode/IRenderNode.h"
#include "Engine/Module/Render/RenderNode/Loader/RenderNodeLoader.h"
#include "Engine/Runtime/Scene/Scene.h"

RenderDAG::RenderDAG() = default;
RenderDAG::~RenderDAG() noexcept = default;

void RenderDAG::setup(const std::string& sceneName, Reference<Scene> scene) {
	JsonAsset json{ std::format("./Game/Core/Scene/{}/RenderPath.json", sceneName) };

	RenderNodeLoader loader;
	loader.setup(scene, renderTargetCollection);

	if(json.cget().is_null()) {
		szgWarning("RenderDAG::setup: Failed to load RenderPath.json: {}", sceneName);
		return;
	}

	renderGraph = loader.entry_point(json.get());
}

void RenderDAG::render_entry_point() const {
	for (const std::unique_ptr<IRenderNode>& node : renderGraph) {
		if (node) {
			node->stack_command();
		}
	}
}
