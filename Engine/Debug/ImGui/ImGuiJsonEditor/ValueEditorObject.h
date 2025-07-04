#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <imgui.h>
#include <imgui_stdlib.h>

#include <bitset>
#include <string>

class Vector2;
class Vector3;

#include <Library/Math/Quaternion.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

namespace ValueEditor {

template<typename T>
struct show_object;

// bool
template<>
struct show_object<bool> {
	inline show_object(const std::string& name_, bool* value_) :
		name(name_),
		value(value_) {
	};
	~show_object() = default;

	__CLASS_DEFAULT_ALL(show_object<bool>)

public:
	inline std::bitset<2> show_gui() {
		ImGui::Checkbox(name.c_str(), value);
		std::bitset<2> result = 0;
		result.set(ImGui::IsItemDeactivated(), 1);
		result.set(ImGui::IsItemActivated(), 0);
		return result;
	};

	std::string_view get_name() const {
		return name;
	}

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

	__CLASS_DEFAULT_ALL(show_object<r32>)

public:
	inline std::bitset<2> show_gui() {
		ImGui::DragFloat(name.c_str(), value, 0.1f);
		std::bitset<2> result = 0;
		result.set(ImGui::IsItemDeactivated(), 1);
		result.set(ImGui::IsItemActivated(), 0);
		return result;
	};

	std::string_view get_name() const {
		return name;
	}

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

	__CLASS_DEFAULT_ALL(show_object<i32>)

public:
	inline std::bitset<2> show_gui() {
		ImGui::DragInt(name.c_str(), value);
		std::bitset<2> result = 0;
		result.set(ImGui::IsItemDeactivated(), 1);
		result.set(ImGui::IsItemActivated(), 0);
		return result;
	};

	std::string_view get_name() const {
		return name;
	}

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

	__CLASS_DEFAULT_ALL(show_object<u32>)

public:
	inline std::bitset<2> show_gui() {
		ImGui::DragScalar(name.c_str(), ImGuiDataType_U32, value);
		std::bitset<2> result = 0;
		result.set(ImGui::IsItemDeactivated(), 1);
		result.set(ImGui::IsItemActivated(), 0);
		return result;
	};

	std::string_view get_name() const {
		return name;
	}

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

	__CLASS_DEFAULT_ALL(show_object<Vector2>)

public:
	inline std::bitset<2> show_gui() {
		ImGui::DragFloat2(name.c_str(), reinterpret_cast<r32*>(value), 0.1f);
		std::bitset<2> result = 0;
		result.set(ImGui::IsItemDeactivated(), 1);
		result.set(ImGui::IsItemActivated(), 0);
		return result;
	};

	std::string_view get_name() const {
		return name;
	}

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

	__CLASS_DEFAULT_ALL(show_object<Vector3>)

public:
	inline std::bitset<2> show_gui() {
		ImGui::DragFloat3(name.c_str(), reinterpret_cast<r32*>(value), 0.1f);
		std::bitset<2> result = 0;
		result.set(ImGui::IsItemDeactivated(), 1);
		result.set(ImGui::IsItemActivated(), 0);
		return result;
	};

	std::string_view get_name() const {
		return name;
	}

private:
	std::string name;
	Vector3* value;
};

// Quaternion
template<>
struct show_object<Quaternion> {
	inline show_object(const std::string& name_, Quaternion* value_) :
		name(name_),
		value(value_) {
	};
	~show_object() = default;

	__CLASS_DEFAULT_ALL(show_object<Quaternion>)

public:
	inline std::bitset<2> show_gui() {
		std::bitset<2> result1 = 0;
		Vector3 rotationL = CVector3::ZERO;
		ImGui::SameLine();
		ImGui::SetNextItemWidth(150);
		if (ImGui::DragFloat3("RotateLocal", &rotationL.x, 1.0f, -180.0f, 180.0f, "")) {
			*value = (*value * Quaternion::EulerDegree(rotationL)).normalize();
		}
		result1.set(ImGui::IsItemDeactivated(), 1);
		result1.set(ImGui::IsItemActivated(), 0);

		std::bitset<2> result2 = 0;
		Vector3 rotationW = CVector3::ZERO;
		ImGui::Indent(23.2f);
		ImGui::SetNextItemWidth(150);
		if (ImGui::DragFloat3("RotateWorld", &rotationW.x, 1.0f, -180.0f, 180.0f, "")) {
			*value *= Quaternion::EulerDegree(rotationW);
			*value = value->normalize();
		}
		result2.set(ImGui::IsItemDeactivated(), 1);
		result2.set(ImGui::IsItemActivated(), 0);
		ImGui::Unindent(23.2f);

		return result1 | result2;
	};

	std::string_view get_name() const {
		return name;
	}

private:
	std::string name;
	Quaternion* value;
};

template<>
struct show_object<std::string> {
	inline show_object(const std::string& name_, std::string* value_) :
		name(name_),
		value(value_) {
	};
	~show_object() = default;

	__CLASS_DEFAULT_ALL(show_object<std::string>)

public:
	inline std::bitset<2> show_gui() {
		ImGui::InputText(name.c_str(), value);
		std::bitset<2> result = 0;
		result.set(ImGui::IsItemDeactivated(), 1);
		result.set(ImGui::IsItemActivated(), 0);
		return result;
	};

	std::string_view get_name() const {
		return name;
	}

private:
	std::string name;
	std::string* value;
};

// debug_gui()関数があるクラス
template<typename T>
concept HasDebugGui = requires(T t, string_literal tag) {
	{ t.debug_gui(tag) } -> std::convertible_to<std::bitset<2>>;
};

template<typename T>
	requires HasDebugGui<T>
struct show_object<T> {
	inline show_object(const std::string& name_, T* value_) :
		name(name_),
		value(value_) {
	};
	~show_object() = default;

	__CLASS_DEFAULT_ALL(show_object<T>)

public:
	inline std::bitset<2> show_gui() {
		return value->debug_gui(name.c_str());
	};

	std::string_view get_name() const {
		return name;
	}

private:
	std::string name;
	T* value;
};

}

#endif // _DEBUG