#include "NodeAnimationPlayer.h"

#include "NodeAnimationResource.h"

#include "Engine/Runtime/WorldClock/WorldClock.h"

void NodeAnimationPlayer::update() {
	timer += WorldClock::DeltaSeconds();
	if (isLoop) {
		timer = std::fmod(timer, nodeAnimation->duration(animationName));
	}
}

Vector3 NodeAnimationPlayer::calculate_scale() const {
	if (nodeAnimation) {
		return nodeAnimation->scale(animationName, nodeName, timer);
	}
	return CVector3::BASIS;
}

Quaternion NodeAnimationPlayer::calculate_rotate() const {
	if (nodeAnimation) {
		return nodeAnimation->rotate(animationName, nodeName, timer);
	}
	return CQuaternion::IDENTITY;
}

Vector3 NodeAnimationPlayer::calculate_translate() const {
	if (nodeAnimation) {
		return nodeAnimation->translate(animationName, nodeName, timer);
	}
	return CVector3::ZERO;
}
