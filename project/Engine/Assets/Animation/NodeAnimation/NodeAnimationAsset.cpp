#include "NodeAnimationAsset.h"

#include "Engine/Debug/Output.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

bool NodeAnimationAsset::load(const std::filesystem::path& filePath) {
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

#ifndef _DEBUG
	animations.reserve(scene->mNumAnimations);
#endif
	for (uint32_t i = 0; i < scene->mNumAnimations; ++i) {
		aiAnimation* aiPAnimation = scene->mAnimations[i];
		// 追加
		auto& newAnimation = animations[aiPAnimation->mName.C_Str()];
		float ticksPerSecond = static_cast<float>(aiPAnimation->mTicksPerSecond);
		// duration算出
		newAnimation.duration = float(aiPAnimation->mDuration) / ticksPerSecond;
		for (uint32_t channelIndex = 0; channelIndex < aiPAnimation->mNumChannels; ++channelIndex) {
			aiNodeAnim* aiNodeAnimation = aiPAnimation->mChannels[channelIndex];
			// Node追加
			NodeAnimation& nodeAnimation = newAnimation.nodeAnimations[aiNodeAnimation->mNodeName.C_Str()];
			// Scale
			for (uint32_t keyIndex = 0; keyIndex < aiNodeAnimation->mNumScalingKeys; ++keyIndex) {
				aiVectorKey& aiKey = aiNodeAnimation->mScalingKeys[keyIndex];
				float time = float(aiKey.mTime) / ticksPerSecond;
				Vector3 value = { aiKey.mValue.x,aiKey.mValue.y,aiKey.mValue.z };
				nodeAnimation.scale.keyframes.emplace(time, value);
			}
			// Rotation
			for (uint32_t keyIndex = 0; keyIndex < aiNodeAnimation->mNumRotationKeys; ++keyIndex) {
				aiQuatKey& aiKey = aiNodeAnimation->mRotationKeys[keyIndex];
				float time = float(aiKey.mTime) / ticksPerSecond;
				Quaternion value = { aiKey.mValue.x, -aiKey.mValue.y, -aiKey.mValue.z, aiKey.mValue.w };
				nodeAnimation.rotate.keyframes.emplace(time, value);
			}
			// Translate
			for (uint32_t keyIndex = 0; keyIndex < aiNodeAnimation->mNumPositionKeys; ++keyIndex) {
				aiVectorKey& aiKey = aiNodeAnimation->mPositionKeys[keyIndex];
				float time = float(aiKey.mTime) / ticksPerSecond;
				Vector3 value = { -aiKey.mValue.x,aiKey.mValue.y,aiKey.mValue.z };
				nodeAnimation.translate.keyframes.emplace(time, value);
			}
		}
	}

	return true;
}

const NodeAnimationAsset::Animation* NodeAnimationAsset::animation(const std::string& animationName) const {
	if (animations.contains(animationName)) {
		return &animations.at(animationName);
	}
	return nullptr;
}

#ifdef _DEBUG
#include <imgui.h>
#include <ranges>
bool NodeAnimationAsset::animation_list_gui(std::string& current) const {
	bool changed = false;
	if (ImGui::BeginCombo("AnimationList", current.c_str())) {
		for (const auto& name : animations | std::views::keys) {
			bool isSelected = current == name;
			if (ImGui::Selectable(name.c_str(), isSelected)) {
				current = name;
				changed = true;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	return changed;
}
#endif // _DEBUG
