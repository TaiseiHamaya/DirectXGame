#include "SkeletonResource.h"

#include "Engine/Debug/Output.h"

#include <Library/Math/Quaternion.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

Affine ToAffine(const aiMatrix4x4& aiMatrix) {
	aiVector3D scale;
	aiQuaternion rotate;
	aiVector3D translate;
	aiMatrix.Decompose(scale, rotate, translate);

	return Affine::FromSRT(
		{ scale.x,scale.y,scale.z },
		Quaternion{ rotate.x, -rotate.y, -rotate.z, rotate.w },
		{ -translate.x,translate.y,translate.z }
	);
}

// 行きがけDFSによるNodeの全探索
std::optional<uint32_t> CreateSkeleton(
	Skeleton& skeleton, // 生成中のSkeleton
	const std::optional<uint32_t>& parentIndex, // 親Index
	const aiNode* node, // 今のNode
	std::unordered_map<std::string, uint32_t>& nodeElementMap // ボーン一覧
) {
	std::optional<uint32_t> thisIndex;
	std::string name = node->mName.C_Str();
	auto& joints = skeleton.joints;
	Joint* joint = nullptr;
	if (nodeElementMap.contains(name)) {
		// Jointの追加
		joint = &joints.emplace_back(
			name,
			Affine{},
			parentIndex,
			std::vector<uint32_t>{}
		);

		thisIndex = static_cast<uint32_t>(joints.size()) - 1;

		nodeElementMap.at(name) = thisIndex.value();
	}

	for (uint32_t nextNodeIndex = 0; nextNodeIndex < node->mNumChildren; ++nextNodeIndex) {
		// 再帰的実行
		std::optional<uint32_t> result = CreateSkeleton(skeleton, thisIndex, node->mChildren[nextNodeIndex], nodeElementMap);
		// 要素が有効な場合のみ追加
		if (result && joint) {
			joint->children.emplace_back(result.value());
		}
	}
	return thisIndex;
}

bool CollectBoneNode(
	std::unordered_map<std::string, uint32_t>& nodeElementMap,
	const aiNode* node,
	const std::unordered_map<std::string, aiBone*>& boneElementsName) {
	bool result = false;
	std::string name = node->mName.C_Str();
	if (boneElementsName.contains(name)) {
		result = true;
	}
	for (uint32_t nextNodeIndex = 0; nextNodeIndex < node->mNumChildren; ++nextNodeIndex) {
		// 再帰的実行
		result = CollectBoneNode(nodeElementMap, node->mChildren[nextNodeIndex], boneElementsName) || result;
	}

	if (result) {
		nodeElementMap.emplace(name, 0);
	}

	return result;
}

bool SkeletonResource::load(const std::filesystem::path& filePath) {
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
	// スケルトン存在しない
	if (!scene->mRootNode || !scene->mNumMeshes) {
		Console("Can't find skeletons. File-\'{}\'\n", filePath.string());
		return false;
	}

	// 使用するボーンの取得
	std::unordered_map<std::string, aiBone*> boneElementsName;
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* pAiMesh = scene->mMeshes[meshIndex];
		if (!pAiMesh->HasBones()) {
			continue;
		}
		for (uint32_t boneIndex = 0; boneIndex < pAiMesh->mNumBones; ++boneIndex) {
			aiBone* bone = pAiMesh->mBones[boneIndex];
			boneElementsName.try_emplace(bone->mName.C_Str(), bone);
		}
	}

	// 使用するNode一覧を取得
	std::unordered_map<std::string, uint32_t> nodeElementMap;
	CollectBoneNode(nodeElementMap, scene->mRootNode, boneElementsName);

	// Node一覧から階層構造を取得
	skeletonData.joints.reserve(nodeElementMap.size());
	CreateSkeleton(skeletonData, std::nullopt, scene->mRootNode, nodeElementMap);

	// SkeletonにInverseBindPoseMatrixを書き込み
	for (Joint& joint : skeletonData.joints) {
		if (!boneElementsName.contains(joint.name)) {
			continue;
		}
		aiBone* bone = boneElementsName.at(joint.name);
		joint.inverseBindPoseAffine = ToAffine(bone->mOffsetMatrix);
	}

	// Meshで使用するNodeとJointのIndexを関連付け
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		const aiMesh* mesh = scene->mMeshes[meshIndex];
		std::vector<uint32_t>& useJointIndexes = skeletonData.useJointIndexesByMeshName[mesh->mName.C_Str()];
		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
			// Bone取得
			std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
			useJointIndexes.emplace_back(nodeElementMap.at(boneName));
		}
	}

	// Weightの書き込み
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		const aiMesh* mesh = scene->mMeshes[meshIndex];
		// Weight用Bufferの生成
		std::vector<VertexInfluenceData> influenceData;
		std::vector<uint32_t> influenceIndexes; // ↑の次のIndexを参照
		influenceData.resize(mesh->mNumVertices);
		influenceIndexes.resize(mesh->mNumVertices, 0);
		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
			// Bone取得
			aiBone* bone = mesh->mBones[boneIndex];
			// 書き込み用データ作成
			for (uint32_t i = 0; i < bone->mNumWeights; ++i) {
				uint32_t vertexIndex = bone->mWeights[i].mVertexId; // 頂点Index
				uint32_t& influenceIndex = influenceIndexes[vertexIndex]; // 書き込み先インデックス
				VertexInfluenceData& influence = influenceData[vertexIndex]; // 書き込み先データ

				if (influenceIndex < VertexInfluenceData::NumMaxInfluence) {
					influence.weights[influenceIndex] = bone->mWeights[i].mWeight;
					influence.jointIndex[influenceIndex] = boneIndex;
					++influenceIndex;
				}
			}
		}
		// GPUに転送
		VertexBuffer<VertexInfluenceData>& influenceBuffer = influenceBuffers[mesh->mName.C_Str()];
		influenceBuffer.write(std::move(influenceData));
	}

	return true;
}

const Skeleton& SkeletonResource::skeleton() const {
	return skeletonData;
}

const VertexBuffer<VertexInfluenceData>* SkeletonResource::weight_influence(const std::string& bindMeshName) const {
	if (influenceBuffers.contains(bindMeshName)) {
		return &influenceBuffers.at(bindMeshName);
	}
	return nullptr;
}

const std::vector<uint32_t>* SkeletonResource::use_joint_indexes(const std::string& bindMeshName) const {
	if (skeletonData.useJointIndexesByMeshName.contains(bindMeshName)) {
		return &skeletonData.useJointIndexesByMeshName.at(bindMeshName);
	}
	return nullptr;
}

uint32_t SkeletonResource::joint_size() const {
	return static_cast<uint32_t>(skeletonData.joints.size());
}
