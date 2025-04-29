#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <imgui.h>

#include <string>

class Vector2;
class Vector3;

namespace ValueEditor {

template<typename T>
struct show_object {
	static_assert(false, "Template paramater (ValueEditor::show_object<T>) is not defined.");
	inline void show_gui() {};
};

// bool
template<>
struct show_object<bool> {
	inline show_object(const std::string& name_, bool* value_) :
		name(name_),
		value(value_) {
	};
	~show_object() = default;

	inline void show_gui() {
		ImGui::Checkbox(name.c_str(), value);
	};

private:
	std::string name;
	bool* value;
};

// r32
template<>
struct show_object<r32> {
	inline show_object(const std::string& name_, r32* value_) :
		name(name_),
		value(value_) {
	};
	~show_object() = default;

	inline void show_gui() {
		ImGui::DragFloat(name.c_str(), value, 0.1f);
	};

private:
	std::string name;
	r32* value;
};

// i32
template<>
struct show_object<i32> {
	inline show_object(const std::string& name_, i32* value_) :
		name(name_),
		value(value_) {
	};
	~show_object() = default;

	inline void show_gui() {
		ImGui::DragInt(name.c_str(), value);
	};

private:
	std::string name;
	i32* value;
};

// u32
template<>
struct show_object<u32> {
	inline show_object(const std::string& name_, u32* value_) :
		name(name_),
		value(value_) {
	};
	~show_object() = default;

	inline void show_gui() {
		ImGui::DragScalar(name.c_str(), ImGuiDataType_U32, value);
	};

private:
	std::string name;
	u32* value;
};

// Vector2
template<>
struct show_object<Vector2> {
	inline show_object(const std::string& name_, Vector2* value_) :
		name(name_),
		value(value_) {
	};
	~show_object() = default;

	inline void show_gui() {
		ImGui::DragFloat2(name.c_str(), reinterpret_cast<r32*>(value), 0.1f);
	};

private:
	std::string name;
	Vector2* value;
};

// Vector3
template<>
struct show_object<Vector3> {
	inline show_object(const std::string& name_, Vector3* value_) :
		name(name_),
		value(value_) {
	};
	~show_object() = default;

	inline void show_gui() {
		ImGui::DragFloat3(name.c_str(), reinterpret_cast<r32*>(value), 0.1f);
	};

private:
	std::string name;
	Vector3* value;
};

// debug_gui()関数があるクラス
template<typename T>
concept HasDebugGui = requires(T t, string_literal tag) {
	{ t.debug_gui(tag) } -> std::same_as<void>;
};

template<typename T>
	requires HasDebugGui<T>
struct show_object<T> {
	inline show_object(const std::string& name_, T* value_) :
		name(name_),
		value(value_) {
	};

	inline void show_gui() {
		value->debug_gui(name.c_str());
	};

private:
	std::string name;
	T* value;
};

}

#endif // _DEBUG