#pragma once

#include <map>
#include <string>
#include <unordered_map>

#include "Library/Utility/Tools/ConstructorMacro.h"

#include <Library/Math/Quaternion.h>
#include <Library/Math/Vector3.h>

struct aiAnimation;

class NodeAnimationAsset final {
public:
	template<typename T>
	struct AnimationCurve {
		std::map<r32, T> keyframes;
	};

	struct NodeAnimation {
		AnimationCurve<Vector3> scale;
		AnimationCurve<Quaternion> rotate;
		AnimationCurve<Vector3> translate;
	};

public:
	NodeAnimationAsset() = default;
	~NodeAnimationAsset() = default;

	__CLASS_NON_COPYABLE(NodeAnimationAsset)

public:
	/// <summary>
	/// ロード関数
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>成功値</returns>
	void load(aiAnimation* aiPAnimation);

public:
	const NodeAnimation& node(const std::string& nodeName) const;
	r32 duration() const { return duration_; }
	bool contains(const std::string& nodeName) const;

private:
	r32 duration_;
	std::unordered_map<std::string, NodeAnimation> nodeAnimations;
};
