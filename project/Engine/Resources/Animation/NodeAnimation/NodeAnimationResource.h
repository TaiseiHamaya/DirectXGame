#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <unordered_map>

#include "Engine/Utility/Tools/ConstructorMacro.h"

#include <Library/Math/Quaternion.h>
#include <Library/Math/Vector3.h>

class NodeAnimationResource final {
private:
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

	const Animation* animation(const std::string& animationName) const;

#ifdef _DEBUG
public:
	bool animation_list_gui(std::string& current) const;
#endif // _DEBUG

private:
#ifdef _DEBUG
	std::map<std::string, Animation> animations;
#else
	std::unordered_map<std::string, Animation> animations;
#endif // _DEBUG
};
