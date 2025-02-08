#include "NodeAnimationPlayer.h"

#include <cmath>
#include <functional>

#include "NodeAnimationAsset.h"
#include "NodeAnimationLibrary.h"

#include "Engine/Runtime/WorldClock/WorldClock.h"

template<typename T>
T CalculateValue(const NodeAnimationAsset::AnimationCurve<T>& animationCurve, float time, std::function<T(const T&, const T&, float)> lerpFunc = std::lerp);

NodeAnimationPlayer::NodeAnimationPlayer(const std::string& fileName, const std::string& animationName_, bool isLoop_) :
	isLoop(isLoop_),
	nodeAnimation(NodeAnimationLibrary::GetAnimation(fileName)) {
	reset_animation(animationName_);
}

void NodeAnimationPlayer::update() noexcept {
	if (!isActive || !animation) {
		return;
	}
	timer += WorldClock::DeltaSeconds() * animationSpeed;
	if (isLoop && is_end()) {
		timer = std::fmod(timer, animation->duration);
	}
}

Vector3 NodeAnimationPlayer::calculate_scale(const std::string& nodeName) const {
	if (animation && animation->nodeAnimations.contains(nodeName)) {
		return CalculateValue<Vector3>(animation->nodeAnimations.at(nodeName).scale, timer, Vector3::Lerp);
	}
	return CVector3::BASIS;
}

Quaternion NodeAnimationPlayer::calculate_rotate(const std::string& nodeName) const {
	if (animation && animation->nodeAnimations.contains(nodeName)) {
		return CalculateValue<Quaternion>(animation->nodeAnimations.at(nodeName).rotate, timer, Quaternion::Slerp);
	}
	return CQuaternion::IDENTITY;
}

Vector3 NodeAnimationPlayer::calculate_translate(const std::string& nodeName) const {
	if (animation && animation->nodeAnimations.contains(nodeName)) {
		return CalculateValue<Vector3>(animation->nodeAnimations.at(nodeName).translate, timer, Vector3::Lerp);
	}
	return CVector3::ZERO;
}

void NodeAnimationPlayer::reset_animation(const std::string& animationName_) {
#ifdef _DEBUG
	animationName = animationName_.empty() ? "UnknownAnimation" : animationName_;
#endif // _DEBUG

	if (nodeAnimation) {
		animation = nodeAnimation->animation(animationName_);
	}
}

void NodeAnimationPlayer::play() noexcept {
	isActive = true;
}

void NodeAnimationPlayer::stop() noexcept {
	isActive = false;
	timer = 0;
}

void NodeAnimationPlayer::pause() noexcept {
	isActive = false;
}

void NodeAnimationPlayer::restart() noexcept {
	stop();
	play();
}

bool NodeAnimationPlayer::is_end() const noexcept {
	return parametric() >= 1.0f;
}

float NodeAnimationPlayer::parametric() const noexcept {
	if (animation) {
		return timer / animation->duration;
	}
	return 1.0f;
}

void NodeAnimationPlayer::set_loop(bool isLoop_) noexcept {
	isLoop = isLoop_;
}

void NodeAnimationPlayer::set_time_force(float timer_) noexcept {
	timer = timer_;
}

void NodeAnimationPlayer::animation_speed(float speed) noexcept {
	animationSpeed = speed;
}

#ifdef _DEBUG
#include <imgui.h>
void NodeAnimationPlayer::debug_gui() {
	if (nodeAnimation) {
		if (nodeAnimation->animation_list_gui(animationName)) {
			reset_animation(animationName);
		}
	}
	if (ImGui::TreeNodeEx(animationName.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Checkbox("Active", &isActive);
		ImGui::Checkbox("Loop", &isLoop);
		if (animation) {
			ImGui::SliderFloat("Timer", &timer, 0, animation->duration, "%.3fs");
		}
		ImGui::DragFloat("AnimationSpeed", &animationSpeed, 0.1f);
		ImGui::TreePop();
	}
}
#endif // _DEBUG

template<typename T>
T CalculateValue(const NodeAnimationAsset::AnimationCurve<T>& animationCurve, float time, std::function<T(const T&, const T&, float)> lerpFunc) {
	const std::map<float, T>& keyframes = animationCurve.keyframes;
	// キーフレームがない場合はTを初期値で返す
	if (keyframes.empty()) {
		return T{};
	}
	// 要素が1もしくは先頭のキーフレームより早い場合は先頭のValueを返す
	if (keyframes.size() == 1 || time <= keyframes.begin()->first) {
		return keyframes.begin()->second;
	}

	// にぶたんしてKeyを探す
	auto endKey = keyframes.upper_bound(time);
	// 末尾より後ろの場合は末尾の値を返す
	if (endKey == keyframes.end()) {
		return keyframes.rbegin()->second;
	}
	// 1つ前のKeyframeを取得
	auto beginKey = std::prev(endKey);
	float parametric = (time - beginKey->first) / (endKey->first - beginKey->first);
	return lerpFunc(beginKey->second, endKey->second, parametric);
}
