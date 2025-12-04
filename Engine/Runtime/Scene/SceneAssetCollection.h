#pragma once

#include <array>
#include <filesystem>
#include <unordered_set>

#include <Library/Utility/Tools/ConstructorMacro.h>

class SceneAssetCollection {
#ifdef DEBUG_FEATURES_ENABLE
	friend class EditorAssetSaver;
#endif // DEBUG_FEATURES_ENABLE

public:
	enum AssetType {
		Texture = 0,
		Mesh,
		Skeleton,
		Animation,
		Audio,
		Shader,
		MSDFFont,

		Max,

		Unknown = -1,
	};

public:
	using AssetListType = std::array<std::unordered_set<std::filesystem::path>, AssetType::Max>;

public:
	SceneAssetCollection() = default;
	~SceneAssetCollection() = default;

	__CLASS_NON_COPYABLE(SceneAssetCollection)

	SceneAssetCollection(const AssetListType& assets, const AssetListType& lazyLoadAssets);

public:
	void load_assets() const;

	static void RegisterLoadQueue(const AssetListType& assets);

private:
	AssetListType assets;
	AssetListType lazyLoadAssets;
};
