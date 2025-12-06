#pragma once

#include <array>
#include <filesystem>
#include <unordered_set>

#include <Library/Utility/Tools/ConstructorMacro.h>

namespace szg {

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

	SZG_CLASS_MOVE_ONLY(SceneAssetCollection)

	SceneAssetCollection(const AssetListType& assets, const AssetListType& lazyLoadAssets);

public:
	/// <summary>
	/// 通常ロードの開始
	/// </summary>
	void load_assets() const;

	/// <summary>
	/// 遅延ロードの開始
	/// </summary>
	void load_lazy_assets() const;

private:
	/// <summary>
	/// BackgroundLoaderに登録する
	/// </summary>
	/// <param name="assets"></param>
	static void RegisterLoadQueue(const AssetListType& assets);

private:
	AssetListType assets;
	AssetListType lazyLoadAssets;
};

}; // szg
