#include "SceneAssetCollection.h"

#include <filesystem>
#include <functional>

#include "Engine/Assets/Animation/NodeAnimation/NodeAnimationLibrary.h"
#include "Engine/Assets/Animation/Skeleton/SkeletonLibrary.h"
#include "Engine/Assets/Audio/AudioLibrary.h"
#include "Engine/Assets/FontAtlasMSDF/FontAtlasMSDFLibrary.h"
#include "Engine/Assets/PolygonMesh/PolygonMeshLibrary.h"
#include "Engine/Assets/Shader/ShaderLibrary.h"
#include "Engine/Assets/Texture/TextureLibrary.h"
#include "Engine/Assets/BackgroundLoader/BackgroundLoader.h"

SceneAssetCollection::SceneAssetCollection(const AssetListType& assets_, const AssetListType& lazyLoadAssets_) {
	assets = assets_;
	lazyLoadAssets = lazyLoadAssets_;
}

void SceneAssetCollection::load_assets() const {
	RegisterLoadQueue(assets);
	BackgroundLoader::WaitEndExecute();
	RegisterLoadQueue(lazyLoadAssets);
}

void SceneAssetCollection::RegisterLoadQueue(const AssetListType& assets) {
	const std::array<std::function<void(const std::filesystem::path&)>, AssetType::Max> loadFunc{
		TextureLibrary::RegisterLoadQue,
		PolygonMeshLibrary::RegisterLoadQue,
		SkeletonLibrary::RegisterLoadQue,
		NodeAnimationLibrary::RegisterLoadQue,
		AudioLibrary::RegisterLoadQue,
		ShaderLibrary::RegisterLoadQue,
		FontAtlasMSDFLibrary::RegisterLoadQue,
	};

	for (u32 i = 0; i < AssetType::Max; ++i) {
		const auto& assetSet = assets[i];
		for (const std::filesystem::path& assetPath : assetSet) {
			loadFunc[i](assetPath);
		}
	}
}
