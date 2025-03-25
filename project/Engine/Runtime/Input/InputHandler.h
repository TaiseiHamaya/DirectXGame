#pragma once

#include <bitset>
#include <unordered_map>
#include <vector>

#include <Engine/Runtime/Input/Input.h>
#include <Engine/Runtime/Clock/WorldTimer.h>

template<typename T>
struct InputHandlerFunction;

template<>
struct InputHandlerFunction<KeyID> {
	inline bool operator()(KeyID id) {
		return Input::IsPressKey(id);
	}
};

template<>
struct InputHandlerFunction<PadID> {
	inline bool operator()(PadID id) {
		return Input::IsPressPad(id);
	};
};

template<>
struct InputHandlerFunction<MouseID> {
	inline bool operator()(MouseID id) {
		return Input::IsPressMouse(id);
	};
};

template<typename T, class InputFunction = InputHandlerFunction<T>>
class InputHandler {
private:
	struct Data {
		WorldTimer timer;
		std::bitset<2> flags;
	};

public:
	InputHandler() = default;
	~InputHandler() = default;

	InputHandler(std::vector<T> keys);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="keys">検知するキー</param>
	void initialize(std::vector<T> keys);

	/// <summary>
	/// 更新処理
	/// </summary>
	void update();

	bool trigger(T id);
	bool press(T id);
	bool release(T id);
	bool idle(T id);

	/// <summary>
	/// 押されている時間
	/// </summary>
	/// <param name="id"></param>
	/// <returns></returns>
	float press_timer(T id);

	/// <summary>
	/// 離れている時間
	/// </summary>
	/// <param name="id"></param>
	/// <returns></returns>
	float idle_timer(T id);

private:
	InputFunction inputter{};
	std::unordered_map<T, Data> data;
};

template<typename T, class InputFunction>
inline InputHandler<T, InputFunction>::InputHandler(std::vector<T> keys) {
	initialize(keys);
}

template<typename T, class InputFunction>
inline void InputHandler<T, InputFunction>::initialize(std::vector<T> keys) {
	data.clear();
	data.reserve(keys.size());
	for (T& key : keys) {
		data.try_emplace(key, WorldTimer{}, false);
	}
}

template<typename T, class InputFunction>
inline void InputHandler<T, InputFunction>::update() {
	for (auto& [id, elem] : data) {
		elem.flags <<= 1;
		elem.flags.set(0, inputter(id));
		if (elem.timer.time() > 0 && elem.flags.count() == 1) {
			elem.timer.set(0);
		}
		else {
			elem.timer.ahead();
		}
	}
}

template<typename T, class InputFunction>
inline bool InputHandler<T, InputFunction>::trigger(T id) {
	if (!data.contains(id)) {
		return false;
	}
	return data[id].flags == std::bitset<2>(0b01);
}

template<typename T, class InputFunction>
inline bool InputHandler<T, InputFunction>::press(T id) {
	if (!data.contains(id)) {
		return false;
	}
	return data[id].flags.test(0);
}

template<typename T, class InputFunction>
inline bool InputHandler<T, InputFunction>::release(T id) {
	if (!data.contains(id)) {
		return false;
	}
	return data[id].flags == std::bitset<2>(0b10);
}

template<typename T, class InputFunction>
inline bool InputHandler<T, InputFunction>::idle(T id) {
	return !press(id);
}

template<typename T, class InputFunction>
inline float InputHandler<T, InputFunction>::press_timer(T id) {
	if (press(id)) {
		return data[id].timer.time();
	}
	return false;
}

template<typename T, class InputFunction>
inline float InputHandler<T, InputFunction>::idle_timer(T id) {
	if (idle(id)) {
		return data[id].timer.time();
	}
	return false;
}
