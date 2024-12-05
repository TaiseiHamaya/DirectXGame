#pragma once

#ifdef _DEBUG

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

// float
template<>
struct show_object<float> {
	inline show_object(const std::string& name_, float* value_) :
		name(name_),
		value(value_) {
	};
	~show_object() = default;

	inline void show_gui() {
		ImGui::DragFloat(name.c_str(), value, 0.1f);
	};

private:
	std::string name;
	float* value;
};

// int
template<>
struct show_object<int> {
	inline show_object(const std::string& name_, int* value_) :
		name(name_),
		value(value_) {
	};
	~show_object() = default;

	inline void show_gui() {
		ImGui::DragInt(name.c_str(), value);
	};

private:
	std::string name;
	int* value;
};

// uint32_t
template<>
struct show_object<uint32_t> {
	inline show_object(const std::string& name_, uint32_t* value_) :
		name(name_),
		value(value_) {
	};
	~show_object() = default;

	inline void show_gui() {
		ImGui::DragScalar(name.c_str(), ImGuiDataType_U32, value);
	};

private:
	std::string name;
	uint32_t* value;
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
		ImGui::DragFloat2(name.c_str(), reinterpret_cast<float*>(value), 0.1f);
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
		ImGui::DragFloat3(name.c_str(), reinterpret_cast<float*>(value), 0.1f);
	};

private:
	std::string name;
	Vector3* value;
};

// debug_gui()関数があるクラス
template<typename T>
concept HasDebugGui = requires(T t, const char* tag) {
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