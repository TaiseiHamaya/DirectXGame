#include "SkeletonResource.h"

#include <unordered_set>

#include "Engine/Debug/Output.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

// 行きがけDFSによるNodeの全探索
std::optional<uint32_t> CreateSkeleton(
	std::vector<Joint>& joints, // 生成中のJoint
	const std::optional<uint32_t>& parentIndex, // 親Index
	aiNode* node, // 今のNode
	const std::unordered_set<std::string>& boneElementsName // ボーン一覧
) {
	if (boneElementsName.contains(node->mName.C_Str())) {
		return std::nullopt;
	}
	// Jointの追加
	auto& newJoint = joints.emplace_back(
		node->mName.C_Str(),
		parentIndex,
		std::vector<uint32_t>{}
	);

	uint32_t thisIndex = static_cast<uint32_t>(joints.size()) - 1;
	for (uint32_t nextNodeIndex = 0; nextNodeIndex < node->mNumChildren; ++nextNodeIndex) {
		// 再帰的実行
		std::optional<uint32_t> result = CreateSkeleton(joints, thisIndex, node->mChildren[nextNodeIndex], boneElementsName);
		// 要素が有効な場合のみ追加
		if (result) {
			newJoint.children.emplace_back(result.value());
		}
	}
	return thisIndex;
}


bool SkeletonResource::load(const std::filesystem::path& filePath) {
	Assimp::Importer importer;
	// 読み込み
	const aiScene* scene = importer.ReadFile(filePath.string().c_str(), aiProcess_MakeLeftHanded);
	// ロード失敗時
	if (importer.GetException() || !scene) {
		Console("Import error. {}\n", importer.GetErrorString());
		return false;
	}
	// スケルトン存在しない
	if (!scene->mRootNode || !scene->mNumMeshes) {
		Console("Can't find skeletons. File-\'{}\'\n", filePath.string());
		return false;
	}

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		std::unordered_set<std::string> boneElementsName;
		aiMesh* pAiMesh = scene->mMeshes[meshIndex];
		if (!pAiMesh->HasBones()) {
			continue;
		}
		// ボーンの一覧を名前で取得し保存
		for (uint32_t boneIndex = 0; boneIndex < pAiMesh->mNumBones; ++boneIndex) {
			boneElementsName.emplace(pAiMesh->mBones[boneIndex]->mName.C_Str());
		}

		// Skeletonの要素追加
		auto& skeleton = skeletons[pAiMesh->mName.C_Str()];

		skeleton.joints.reserve(boneElementsName.size());
		// 再帰関数の開始
		CreateSkeleton(skeleton.joints, std::nullopt, scene->mRootNode, boneElementsName);

		skeleton.root = std::to_address(skeleton.joints.begin());
	}

	return true;
}

const Skeleton* SkeletonResource::skeleton(const std::string& bindMeshName) const {
	if (skeletons.contains(bindMeshName)) {
		return &skeletons.at(bindMeshName);
	}
	return nullptr;
}
