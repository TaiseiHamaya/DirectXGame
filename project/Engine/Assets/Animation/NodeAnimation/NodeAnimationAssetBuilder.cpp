#include "NodeAnimationAssetBuilder.h"

#include "./NodeAnimationAsset.h"
#include "./NodeAnimationLibrary.h"
#include "Engine/Debug/Output.h"

#include "Library/Utility/Tools/SmartPointer.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

NodeAnimationAssetBuilder::NodeAnimationAssetBuilder(const std::filesystem::path& filePath_) {
	filePath = filePath_;
}

void NodeAnimationAssetBuilder::preprocess() {
	nodeAnimationData.clear();
	//nodeAnimationData = eps::CreateShared<NodeAnimationAsset>();
}

bool NodeAnimationAssetBuilder::run() {
	Assimp::Importer importer;
	// 読み込み
	const aiScene* scene = importer.ReadFile(filePath.string().c_str(),
		aiProcess_FlipUVs |
		aiProcess_FlipWindingOrder |
		aiProcess_LimitBoneWeights
	);
	// ロード失敗時
	if (importer.GetException() || !scene) {
		Console("Import error. {}\n", importer.GetErrorString());
		return false;
	}
	// アニメーションが存在しない
	if (!scene->HasAnimations()) {
		Console("Can't find animation. File-\'{}\'\n", filePath.string());
		return false;
	}

	nodeAnimationData.resize(scene->mNumAnimations);

	std::string fileName = filePath.filename().string();

	for (uint32_t i = 0; i < scene->mNumAnimations; ++i) {
		BuildData& write = nodeAnimationData[i];
		aiAnimation* aiPAnimation = scene->mAnimations[i];
		write.name = std::format("{}-{}", fileName, aiPAnimation->mName.C_Str());
		write.asset = eps::CreateShared<NodeAnimationAsset>();
		write.asset->load(aiPAnimation);
	}
	
	return true;
}

void NodeAnimationAssetBuilder::postprocess() {
	// Do nothing
}

void NodeAnimationAssetBuilder::transfer() {
	for (const BuildData& data : nodeAnimationData) {
		NodeAnimationLibrary::Transfer(data.name, data.asset);
	}
}
