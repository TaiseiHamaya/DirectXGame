#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <imgui.h>
#include <imgui_stdlib.h>

#include <bitset>
#include <string>

class Vector2;
class Vector3;
class Color3;
class Color4;

#include <Library/Math/Quaternion.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

namespace ValueEditor {

template<typename T>
struct show_object;

// bool
template<>
struct show_object<bool> {
	inline show_object(const std::string& name_) :
		name(name_) {
	};
	~show_object() = default;

	__CLASS_DEFAULT_ALL(show_object<bool>)

public:
	inline std::bitset<2> show_gui(bool& value) const {
		ImGui::Checkbox(name.c_str(), &value);
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
};

// r32
template<>
struct show_object<r32> {
	inline show_object(const std::string& name_) :
		name(name_) {
	};
	~show_object() = default;

	__CLASS_DEFAULT_ALL(show_object<r32>)

public:
	inline std::bitset<2> show_gui(r32& value) const {
		ImGui::DragFloat(name.c_str(), &value, 0.1f);
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
};

// i32
template<>
struct show_object<i32> {
	inline show_object(const std::string& name_) :
		name(name_) {
	};
	~show_object() = default;

	__CLASS_DEFAULT_ALL(show_object<i32>)

public:
	inline std::bitset<2> show_gui(i32& value) const {
		constexpr i32 step = 1;
		constexpr i32 fastStep = 10;
		ImGui::InputScalar(name.c_str(), ImGuiDataType_S32, &value, &step, &fastStep);
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
};

// u32
template<>
struct show_object<u32> {
	inline show_object(const std::string& name_) :
		name(name_) {
	};
	~show_object() = default;

	__CLASS_DEFAULT_ALL(show_object<u32>)

public:
	inline std::bitset<2> show_gui(u32& value) const {
		constexpr i32 step = 1;
		constexpr i32 fastStep = 10;
		ImGui::InputScalar(name.c_str(), ImGuiDataType_U32, &value, &step, &fastStep);
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
};

// Vector2
template<>
struct show_object<Vector2> {
	inline show_object(const std::string& name_) :
		name(name_) {
	};
	~show_object() = default;

	__CLASS_DEFAULT_ALL(show_object<Vector2>)

public:
	inline std::bitset<2> show_gui(Vector2& value) const {
		ImGui::DragFloat2(name.c_str(), reinterpret_cast<r32*>(&value), 0.1f);
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
};

// Vector3
template<>
struct show_object<Vector3> {
	inline show_object(const std::string& name_) :
		name(name_) {
	};
	~show_object() = default;

	__CLASS_DEFAULT_ALL(show_object<Vector3>)

public:
	inline std::bitset<2> show_gui(Vector3& value) const {
		ImGui::DragFloat3(name.c_str(), reinterpret_cast<r32*>(&value), 0.1f);
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
};

// Quaternion
template<>
struct show_object<Quaternion> {
	inline show_object(const std::string& name_) :
		name(name_) {
	};
	~show_object() = default;

	__CLASS_DEFAULT_ALL(show_object<Quaternion>)

public:
	inline std::bitset<2> show_gui(Quaternion& value) const {
		std::bitset<2> result1 = 0;
		Vector3 rotationL = CVector3::ZERO;
		ImGui::SetNextItemWidth(150);
		if (ImGui::DragFloat3("RotateLocal", &rotationL.x, 1.0f, -180.0f, 180.0f, "")) {
			value = (value * Quaternion::EulerDegree(rotationL)).normalize();
		}
		result1.set(ImGui::IsItemDeactivated(), 1);
		result1.set(ImGui::IsItemActivated(), 0);

		std::bitset<2> result2 = 0;
		Vector3 rotationW = CVector3::ZERO;
		ImGui::SetNextItemWidth(150);
		if (ImGui::DragFloat3("RotateWorld", &rotationW.x, 1.0f, -180.0f, 180.0f, "")) {
			value *= Quaternion::EulerDegree(rotationW);
			value = value.normalize();
		}
		result2.set(ImGui::IsItemDeactivated(), 1);
		result2.set(ImGui::IsItemActivated(), 0);

		return result1 | result2;
	};

	std::string_view get_name() const {
		return name;
	}

private:
	std::string name;
};

template<>
struct show_object<std::string> {
	inline show_object(const std::string& name_) :
		name(name_) {
	};
	~show_object() = default;

	__CLASS_DEFAULT_ALL(show_object<std::string>)

public:
	inline std::bitset<2> show_gui(std::string& value) const {
		ImGui::InputText(name.c_str(), &value);
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
};

template<>
struct show_object<Color3> {
	inline show_object(const std::string& name_) :
		name(name_) {
	};
	~show_object() = default;

	__CLASS_DEFAULT_ALL(show_object<Color3>)

public:
	inline std::bitset<2> show_gui(Color3& value) const {
		ImGui::ColorEdit3(name.c_str(), reinterpret_cast<r32*>(&value), ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB);
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
};

template<>
struct show_object<Color4> {
	inline show_object(const std::string& name_) :
		name(name_) {
	};
	~show_object() = default;

	__CLASS_DEFAULT_ALL(show_object<Color4>)

public:
	inline std::bitset<2> show_gui(Color4& value) const {
		ImGui::ColorEdit4(name.c_str(), reinterpret_cast<r32*>(&value), ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_InputRGB);
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
};

// debug_gui()関数があるクラス
template<typename T>
concept HasDebugGui = requires(T t, string_literal tag) {
	{ t.debug_gui(tag) } -> std::convertible_to<std::bitset<2>>;
};

template<typename T>
	requires HasDebugGui<T>
struct show_object<T> {
	inline show_object(const std::string& name_) :
		name(name_) {
	};
	~show_object() = default;

	__CLASS_DEFAULT_ALL(show_object<T>)

public:
	inline std::bitset<2> show_gui(T& value) const {
		return value.debug_gui(name.c_str());
	};

	std::string_view get_name() const {
		return name;
	}

private:
	std::string name;
};

}

#endif // _DEBUG