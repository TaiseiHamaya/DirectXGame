#include "PostEffectPSOLoader.h"

#include "Engine/Module/Render/RenderPSO/Posteffect/ChromaticAberration/ChromaticAberrationNode.h"
#include "Engine/Module/Render/RenderPSO/Posteffect/Grayscale/GrayscaleNode.h"
#include "Engine/Module/Render/RenderPSO/Posteffect/Outline/OutlineNode.h"
#include "Engine/Module/Render/RenderPSO/Posteffect/RadialBlur/RadialBlurNode.h"

std::unique_ptr<PostEffectPSO> PostEffectPSOLoader::entry_point(const nlohmann::json& psoJson) {
	if (!psoJson.contains("Type") || !psoJson["Type"].is_string()) {
		return nullptr;
	}
	const std::string& type = psoJson["Type"].get<std::string>();
	if (type == "ChromaticAberration") {
		auto node = std::make_unique<ChromaticAberrationNode>();
		node->initialize();
		return node;
	}
	else if (type == "Grayscale") {
		auto node = std::make_unique<GrayscaleNode>();
		node->initialize();
		return node;
	}
	else if (type == "Outline") {
		auto node = std::make_unique<OutlineNode>();
		node->initialize();
		return node;
	}
	else if (type == "RadialBlur") {
		auto node = std::make_unique<RadialBlurNode>();
		node->initialize();
		return node;
	}

	if (psoJson.contains("Value")) {

	}

	return nullptr;
}
