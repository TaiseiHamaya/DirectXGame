#pragma once

#include <filesystem>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>

#include "Engine/Utility/Tools/ConstructorMacro.h"

#include <Library/Math/Quaternion.h>
#include <Library/Math/Vector3.h>

class NodeAnimationResource final {
private:
	//template<typename T>
	//struct Keyframe {
	//	float time;
	//	T value{};
	//};

	template<typename T>
	struct AnimationCurve {
		std::map<float, T> keyframes;
	};

	struct NodeAnimation {
		AnimationCurve<Vector3> scale;
		AnimationCurve<Quaternion> rotate;
		AnimationCurve<Vector3> translate;
	};

public:
	struct Animation {
		float duration;
		std::unordered_map<std::string, NodeAnimation> nodeAnimations;
	};

public:
	NodeAnimationResource() = default;
	~NodeAnimationResource() = default;

	__NON_COPYABLE_CLASS(NodeAnimationResource)

public:
	/// <summary>
	/// ロード関数
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>成功値</returns>
	bool load(const std::filesystem::path& filePath);

	Vector3 scale(const std::string& animationName, const std::string& nodeName, float time);
	Quaternion rotate(const std::string& animationName, const std::string& nodeName, float time);
	Vector3 translate(const std::string& animationName, const std::string& nodeName, float time);

public:
	float duration(const std::string& animationName);

private:

private:
	std::unordered_map<std::string, Animation> animations;
};
