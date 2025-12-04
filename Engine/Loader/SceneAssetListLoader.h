#pragma once

#include <json.hpp>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/Runtime/Scene/SceneAssetCollection.h"

class SceneAssetListLoader {
public:
	SceneAssetListLoader() = default;
	~SceneAssetListLoader() = default;

	__CLASS_NON_COPYABLE(SceneAssetListLoader)

public:
	SceneAssetCollection load(const std::string& sceneName);

private:
	void load_asset_list(
		SceneAssetCollection::AssetListType& assets,
		const nlohmann::json& json
	);

public:
	static constexpr std::array<string_literal, SceneAssetCollection::AssetType::Max> AssetTypeNames = {
		"Textures",
		"Meshes",
		"Skeletons",
		"Animations",
		"Audios",
		"Shaders",
		"MSDFFonts",
	};
};
