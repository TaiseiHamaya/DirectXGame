#pragma once

#include <memory>
#include <string>

#include "Engine/Utility/Tools/ConstructorMacro.h"

#include <Library/Math/Vector3.h>
#include <Library/Math/Quaternion.h>

class NodeAnimationResource;

class NodeAnimationPlayer {
public:
	NodeAnimationPlayer() = default;
	~NodeAnimationPlayer() = default;

	__NON_COPYABLE_CLASS(NodeAnimationPlayer)

public:
	void update();

	Vector3 calculate_scale() const;
	Quaternion calculate_rotate() const;
	Vector3 calculate_translate() const;

public:
	bool isLoop;
	float timer;
	std::string animationName;
	std::string nodeName;
	std::shared_ptr<NodeAnimationResource> nodeAnimation;
};
