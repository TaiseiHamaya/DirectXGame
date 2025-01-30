#pragma once

#include <functional>

#include "Engine/Runtime/WorldClock/WorldClock.h"

template<class Type>
class TimedCall {
public:
	TimedCall() = default;
	TimedCall(std::function<Type>&& function_, float time_);
	~TimedCall() = default;

	void update();

	bool is_finished() const;

	void restart(float time);

#ifdef _DEBUG
public:
	void debug_gui();
#endif // _DEBUG

private:
	std::function<Type> function;
	float time;
	bool isFinished = false;
};

template<class Type>
inline TimedCall<Type>::TimedCall(std::function<Type>&& function_, float time_) {
	function = function_;
	time = time_;
}

template<class Type>
inline void TimedCall<Type>::update() {
	time -= WorldClock::DeltaSeconds();
	if (time <= 0) {
		isFinished = true;
		function();
	}
}

template<class Type>
inline bool TimedCall<Type>::is_finished() const {
	return isFinished;
}

template<class Type>
inline void TimedCall<Type>::restart(float time_) {
	time = time_;
	isFinished = false;
}

#ifdef _DEBUG

#include <imgui.h>
#include <format>

template<class Type>
inline void TimedCall<Type>::debug_gui() {
	if (ImGui::CollapsingHeader(std::format("TimedCall##{}", (void*)this).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Text(std::format("Finished : {:s}", isFinished).c_str());
		ImGui::Text(std::format("Timer : {}", time).c_str());
	}
}
#endif // _DEBUG
