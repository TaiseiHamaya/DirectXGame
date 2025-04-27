#include "NodeAnimationAsset.h"

#include <assimp/anim.h>
#include <assimp/quaternion.h>
#include <assimp/types.h>
#include <assimp/vector3.h>

void NodeAnimationAsset::load(aiAnimation* aiPAnimation) {
	r32 ticksPerSecond = static_cast<r32>(aiPAnimation->mTicksPerSecond);
	// duration算出
	duration_ = r32(aiPAnimation->mDuration) / ticksPerSecond;
	for (u32 channelIndex = 0; channelIndex < aiPAnimation->mNumChannels; ++channelIndex) {
		aiNodeAnim* aiNodeAnimation = aiPAnimation->mChannels[channelIndex];
		// Node追加
		NodeAnimation& nodeAnimation = nodeAnimations[aiNodeAnimation->mNodeName.C_Str()];
		// Scale
		for (u32 keyIndex = 0; keyIndex < aiNodeAnimation->mNumScalingKeys; ++keyIndex) {
			aiVectorKey& aiKey = aiNodeAnimation->mScalingKeys[keyIndex];
			r32 time = r32(aiKey.mTime) / ticksPerSecond;
			Vector3 value = { aiKey.mValue.x,aiKey.mValue.y,aiKey.mValue.z };
			nodeAnimation.scale.keyframes.emplace(time, value);
		}
		// Rotation
		for (u32 keyIndex = 0; keyIndex < aiNodeAnimation->mNumRotationKeys; ++keyIndex) {
			aiQuatKey& aiKey = aiNodeAnimation->mRotationKeys[keyIndex];
			r32 time = r32(aiKey.mTime) / ticksPerSecond;
			Quaternion value = { aiKey.mValue.x, -aiKey.mValue.y, -aiKey.mValue.z, aiKey.mValue.w };
			nodeAnimation.rotate.keyframes.emplace(time, value);
		}
		// Translate
		for (u32 keyIndex = 0; keyIndex < aiNodeAnimation->mNumPositionKeys; ++keyIndex) {
			aiVectorKey& aiKey = aiNodeAnimation->mPositionKeys[keyIndex];
			r32 time = r32(aiKey.mTime) / ticksPerSecond;
			Vector3 value = { -aiKey.mValue.x,aiKey.mValue.y,aiKey.mValue.z };
			nodeAnimation.translate.keyframes.emplace(time, value);
		}
	}
}

const NodeAnimationAsset::NodeAnimation& NodeAnimationAsset::node(const std::string& nodeName) const {
	return nodeAnimations.at(nodeName);
}

bool NodeAnimationAsset::contains(const std::string& nodeName) const {
	return nodeAnimations.contains(nodeName);
}
