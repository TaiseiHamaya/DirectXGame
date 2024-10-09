#pragma once

#include <optional>
#include <functional>
#include <unordered_map>

template<typename T>
concept Object = std::is_object_v<T>;

template<Object T>
class Behavior final {
	struct BehaviorFunctions {
		std::function<void(void)> initializeFunction;
		std::function<void(void)> updateFunction;
	};

public:
	Behavior() = default;
	~Behavior() = default;

	Behavior(const Behavior&) = delete;
	Behavior& operator=(const Behavior&) = delete;
	Behavior(Behavior&&) = default;
	Behavior& operator=(Behavior&&) = default;

public:
	void initalize(const T& value) noexcept;
	void update();

public:
	std::function<void(const T&)> get_request_function();
	void behavior_request(const T& request_) noexcept { request = request_; };
	void add_list(
		const T& key,
		std::function<void(void)> initializeFunction,
		std::function<void(void)> updateFunction
	);

#ifdef _DEBUG
public:
	void debug_gui();
#endif // _DEBUG

private:
	T behavior;
	std::optional<T> request;
	std::unordered_map<T, BehaviorFunctions> behaviorList;
};

template<Object T>
inline void Behavior<T>::initalize(const T& value) noexcept {
	behavior_request(value);
}

template<Object T>
inline void Behavior<T>::update() {
	if (request.has_value()) {
		if (behaviorList.contains(request.value())) {
			behavior = request.value();
			behaviorList.at(behavior).initializeFunction();
		}
		request = std::nullopt;
	}

	if (behaviorList.contains(behavior)) {
		behaviorList[behavior].updateFunction();
	}
}

template<Object T>
inline std::function<void(const T&)> Behavior<T>::get_request_function() {
	return std::bind(&Behavior<T>::behavior_request, this, std::placeholders::_1);
}

template<Object T>
inline void Behavior<T>::add_list(const T& key, std::function<void(void)> initializeFunction, std::function<void(void)> updateFunction) {
	behaviorList.try_emplace(
		key,
		BehaviorFunctions{ initializeFunction, updateFunction }
	);
}

#ifdef _DEBUG

#include <imgui.h>
#include <format>

template<Object T>
inline void Behavior<T>::debug_gui() {
	if constexpr (std::is_enum_v<T>) {
		// Enumの場合、キャストが必要
		using underlying = std::underlying_type<T>::type;
		underlying value = static_cast<underlying>(behavior);
		ImGui::Text(std::format("Behavior : {}", value).c_str());
	}
	else {
		// それ以外の場合は通常format(ただし、formatterが必要)
		ImGui::Text(std::format("Behavior : {}", behavior).c_str());
	}
}
#endif // _DEBUG
