#include "EditorAssetSaver.h"

#include <memory>

#include "../Window/RenderDagImNode/PostEffectImNode.h"
#include "Engine/Assets/Json/JsonAsset.h"
#include "Engine/Debug/Editor/RemoteObject/RemoteSceneObject.h"
#include "Engine/Debug/Editor/Window/EditorRenderDAG.h"
#include "Engine/Loader/RenderPath/RenderNodeType.h"
#include "Engine/Loader/SceneAssetListLoader.h"

void EditorAssetSaver::setup(Reference<const EditorRenderDAG> dagEditor_, Reference<const RemoteSceneObject> scene_) {
	dagEditor = dagEditor_;
	scene = scene_;
}

void EditorAssetSaver::save(const std::filesystem::path& filePath) {
	collect_shaders();

	save_to_json(filePath);
}

void EditorAssetSaver::collect_shaders() {
	if (!dagEditor) {
		return;
	}

	const std::unordered_map<u64, EditorRenderDAG::DAGNodeType>& nodes = dagEditor->nodes_imm();

	for (const auto& [id, node] : nodes) {
		RenderNodeType nodeType = static_cast<RenderNodeType>(node.index());
		if (nodeType != RenderNodeType::PostEffect) {
			continue;
		}

		std::shared_ptr<PostEffectImNode> postEffectNode = std::get<std::shared_ptr<PostEffectImNode>>(node);
		PostEffectType peType = postEffectNode->post_effect_type();
		std::filesystem::path pixelShaderFile;
		switch (peType) {
		case PostEffectType::ChromaticAberration:
			pixelShaderFile = L"[[szg]]/PostEffect/ChromaticAberration.PS.hlsl";
			break;
		case PostEffectType::Grayscale:
			pixelShaderFile = L"[[szg]]/PostEffect/Grayscale.PS.hlsl";
			break;
		case PostEffectType::Outline:
			pixelShaderFile = L"[[szg]]/PostEffect/Outline.PS.hlsl";
			break;
		case PostEffectType::RadialBlur:
			pixelShaderFile = L"[[szg]]/PostEffect/RadialBlur.PS.hlsl";
			break;
		case PostEffectType::DownSampling:
			pixelShaderFile = L"[[szg]]/PostEffect/CopyTexture.PS.hlsl";
			break;
		case PostEffectType::TextureBlend2:
			// TODO: まだない
			break;
		case PostEffectType::TextureBlend4:
			pixelShaderFile = L"[[szg]]/PostEffect/MargeTexture4.PS.hlsl";
			break;
		case PostEffectType::LuminanceExtraction:
			pixelShaderFile = L"[[szg]]/PostEffect/LuminanceExtraction/LuminanceExtraction.PS.hlsl";
			break;
		case PostEffectType::GaussianBlur:
			pixelShaderFile = L"[[szg]]/PostEffect/GaussianBlur/GaussianBlur.PS.hlsl";
			break;
		case PostEffectType::Bloom:
			pixelShaderFile = L"[[szg]]/PostEffect/Bloom/Bloom.PS.hlsl";
			break;
		default:
			continue;
			break;
		}

		sceneAssetCollection.assets[SceneAssetCollection::AssetType::Shader].emplace(pixelShaderFile.string());
	}
}

void EditorAssetSaver::save_to_json(const std::filesystem::path& filePath) {
	JsonAsset json{ filePath / "Assets.json" };

	nlohmann::json assets = nlohmann::json::object();
	for (u32 i = 0; i < SceneAssetCollection::AssetType::Max; ++i) {
		const std::unordered_set<std::filesystem::path>& assetList = sceneAssetCollection.assets[i];
		nlohmann::json assetArray = nlohmann::json::array();
		for (const auto& asset : assetList) {
			assetArray.emplace_back(asset);
		}
		assets[SceneAssetListLoader::AssetTypeNames[i]] = assetArray;
	}

	json.get().clear();
	json.get() = assets;

	json.save();
}
