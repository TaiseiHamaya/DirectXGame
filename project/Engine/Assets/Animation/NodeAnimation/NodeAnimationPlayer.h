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

	__CLASS_NON_COPYABLE(NodeAnimationPlayer)

public:
	void update() noexcept;

	Vector3 calculate_scale(const std::string& nodeName) const;
	Quaternion calculate_rotate(const std::string& nodeName) const;
	Vector3 calculate_translate(const std::string& nodeName) const;

	void reset_animation(const std::string& animationName);

	void play() noexcept;
	void stop() noexcept;
	void pause() noexcept;
	void restart() noexcept;
	bool is_end() const noexcept;
	float parametric() const noexcept;

	void set_loop(bool isLoop) noexcept;
	void set_time_force(float timer) noexcept;
	void animation_speed(float speed = 1.0f) noexcept;

#ifdef _DEBUG
public:
	void debug_gui();
#endif // _DEBUG

private:
	bool isLoop{ false };
	bool isActive{ true };
	float timer{ 0 };
	float animationSpeed{ 1.0f };
	std::shared_ptr<const NodeAnimationAsset> nodeAnimation;

#ifdef _DEBUG
	std::string animationName;
#endif // _DEBUG
};
