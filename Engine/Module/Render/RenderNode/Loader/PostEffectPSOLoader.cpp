#include "PostEffectPSOLoader.h"

#include "Engine/Module/Manager/RuntimeStorage/RuntimeStorage.h"
#include "Engine/Module/Render/RenderPSO/Posteffect/ChromaticAberration/ChromaticAberrationNode.h"
#include "Engine/Module/Render/RenderPSO/Posteffect/Grayscale/GrayscaleNode.h"
#include "Engine/Module/Render/RenderPSO/Posteffect/Outline/OutlineNode.h"
#include "Engine/Module/Render/RenderPSO/Posteffect/RadialBlur/RadialBlurNode.h"

#define VECTOR2_SERIALIZER
#include <Engine/Assets/Json/JsonSerializer.h>

std::unique_ptr<PostEffectPSO> PostEffectPSOLoader::entry_point(const nlohmann::json& psoJson) {
	if (!psoJson.contains("Type") || !psoJson["Type"].is_string()) {
		return nullptr;
	}

	RuntimeStorage::ValueGroup& postEffectValueGroup = RuntimeStorage::GetValue("PostEffect");
	std::any value;

	std::unique_ptr<PostEffectPSO> node = nullptr;
	const std::string& type = psoJson["Type"].get<std::string>();
	if (type == "ChromaticAberration") {
		// ----- 色収差 -----
		node = std::make_unique<ChromaticAberrationNode>();
		node->initialize();

		if (psoJson.contains("Value")) {
			value = psoJson["Value"].get<Vector2>();
		}
	}
	else if (type == "Grayscale") {
		// ----- グレースケール -----
		node = std::make_unique<GrayscaleNode>();
		node->initialize();
		
		if (psoJson.contains("Value")) {
			value = psoJson["Value"].get<u32>();
		}
	}
	else if (type == "Outline") {
		// ----- アウトライン -----
		node = std::make_unique<OutlineNode>();
		node->initialize();
	}
	else if (type == "RadialBlur") {
		// ----- ラジアルブラー -----
		node = std::make_unique<RadialBlurNode>();
		node->initialize();
		
		if (psoJson.contains("Value")) {
			BlurInfo blurInfo{};
			blurInfo.center = psoJson["Value"].value("Center", Vector2{ 0.5f, 0.5f });
			blurInfo.weight = psoJson["Value"].value("Weight", 0.4f);
			blurInfo.length = psoJson["Value"].value("Length", 0.1f);
			blurInfo.sampleCount = psoJson["Value"].value("SampleCount", 1u);
			value = blurInfo;
		}
	}

	// ランタイム参照可能にするチェック
	if (psoJson.contains("UseRuntime") && psoJson["UseRuntime"].get<bool>()) {
		if (psoJson.contains("Name") && psoJson["Name"].is_string() && !value.has_value()) {
			postEffectValueGroup[psoJson["Name"]] = std::move(value);
			node->setup(psoJson["Name"]);
		}
	}

	return nullptr;
}
