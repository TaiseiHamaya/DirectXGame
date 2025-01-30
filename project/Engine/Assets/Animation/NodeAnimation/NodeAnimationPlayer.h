#pragma once

#include <memory>
#include <string>

#include "./NodeAnimationAsset.h"

#include <Library/Utility/Tools/ConstructorMacro.h>
#include <Library/Math/Quaternion.h>
#include <Library/Math/Vector3.h>

class NodeAnimationPlayer {
public:
	NodeAnimationPlayer() = default;
	NodeAnimationPlayer(const std::string& fileName, const std::string& animationName, bool isLoop);
	~NodeAnimationPlayer() = default;

	__NON_COPYABLE_CLASS(NodeAnimationPlayer)

public:
	void update();

	Vector3 calculate_scale(const std::string& nodeName) const;
	Quaternion calculate_rotate(const std::string& nodeName) const;
	Vector3 calculate_translate(const std::string& nodeName) const;

	void reset_animation(const std::string& animationName);

	void play();
	void stop();
	void pause();
	void restart();
	bool is_end() const;
	float parametric() const;

	void set_loop(bool isLoop);
	void animation_speed(float speed = 1.0f);

#ifdef _DEBUG
public:
	void debug_gui();
#endif // _DEBUG

private:
	bool isLoop{ false };
	bool isActive{ true };
	float timer{ 0 };
	float animationSpeed{ 1.0f };
	const NodeAnimationAsset::Animation* animation{ nullptr };
	const NodeAnimationAsset::Animation* nextAnimation{ nullptr };
	std::shared_ptr<const NodeAnimationAsset> nodeAnimation;

#ifdef _DEBUG
	std::string animationName;
#endif // _DEBUG
};
