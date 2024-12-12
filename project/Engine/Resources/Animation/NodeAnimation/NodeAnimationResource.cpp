#include "NodeAnimationResource.h"

#include <functional>
#include <cmath>

#include "Engine/Debug/Output.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

template<typename T>
T CalculateValue(const NodeAnimationResource::AnimationCurve<T>& animationCurve, float time, std::function<T(const T&, const T&, float)> lerpFunc = std::lerp);

bool NodeAnimationResource::load(const std::filesystem::path& filePath) {
	Assimp::Importer importer;
	// 読み込み
	const aiScene* scene = importer.ReadFile(filePath.string().c_str(), aiProcess_MakeLeftHanded);
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

	animations.reserve(scene->mNumAnimations);
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
				Quaternion value = { aiKey.mValue.x, aiKey.mValue.y, aiKey.mValue.z, aiKey.mValue.w };
				nodeAnimation.rotate.keyframes.emplace(time, value);
			}
			// Translate
			for (uint32_t keyIndex = 0; keyIndex < aiNodeAnimation->mNumPositionKeys; ++keyIndex) {
				aiVectorKey& aiKey = aiNodeAnimation->mPositionKeys[keyIndex];
				float time = float(aiKey.mTime) / ticksPerSecond;
				Vector3 value = { aiKey.mValue.x,aiKey.mValue.y,aiKey.mValue.z };
				nodeAnimation.translate.keyframes.emplace(time, value);
			}
		}
	}

	return true;
}

Vector3 NodeAnimationResource::scale(const std::string& animationName, const std::string& nodeName, float time) {
	if (!animations.contains(animationName) || !animations.at(animationName).nodeAnimations.contains(nodeName)) {
		return CVector3::ZERO;
	}
	return CalculateValue<Vector3>(animations.at(animationName).nodeAnimations.at(nodeName).scale, time, Vector3::Lerp);
}

Quaternion NodeAnimationResource::rotate(const std::string& animationName, const std::string& nodeName, float time) {
	if (!animations.contains(animationName) || !animations.at(animationName).nodeAnimations.contains(nodeName)) {
		return CQuaternion::IDENTITY;
	}
	return CalculateValue<Quaternion>(animations.at(animationName).nodeAnimations.at(nodeName).rotate, time, Quaternion::Slerp);
}

Vector3 NodeAnimationResource::translate(const std::string& animationName, const std::string& nodeName, float time) {
	if (!animations.contains(animationName) || !animations.at(animationName).nodeAnimations.contains(nodeName)) {
		return CVector3::ZERO;
	}
	return CalculateValue<Vector3>(animations.at(animationName).nodeAnimations.at(nodeName).translate, time, Vector3::Lerp);
}

float NodeAnimationResource::duration(const std::string& animationName) {
	if (animations.contains(animationName)) {
		return animations.at(animationName).duration;
	}
	return 0.0f;
}

template<typename T>
T CalculateValue(const NodeAnimationResource::AnimationCurve<T>& animationCurve, float time, std::function<T(const T&, const T&, float)> lerpFunc) {
	const std::map<float, T>& keyframes = animationCurve.keyframes;
	if (keyframes.empty()) {
		return T{};
	}
	if (keyframes.size() == 1 || time <= keyframes.begin()->first) {
		return keyframes.begin()->second;
	}

	auto endKey = keyframes.upper_bound(time);
	// 末尾よりあとの場合は末尾の値を返す
	if (endKey == keyframes.end()) {
		return keyframes.rbegin()->second;
	}
	// 1つ前のKeyframeを取得
	auto beginKey = std::prev(endKey);
	float parametric = (time - beginKey->first) / (endKey->first - beginKey->first);
	return lerpFunc(beginKey->second, endKey->second, parametric);
}
