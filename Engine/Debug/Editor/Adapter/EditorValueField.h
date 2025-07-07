#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <concepts>
#include <format>
#include <string>

#include <json.hpp>

#include <Library/Math/Quaternion.h>
#include <Library/Math/Transform2D.h>
#include <Library/Math/Transform3D.h>
#include <Library/Math/Vector3.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/Debug/Editor/Command/EditorValueChangeCommandHandler.h"
#include "Engine/Debug/Editor/Command/EditorCommandScope.h"
#include <Engine/Debug/ImGui/ImGuiJsonEditor/ValueEditorObject.h>

#define TRANSFORM3D_SERIALIZER
#define TRANSFORM2D_SERIALIZER
#include "Engine/Assets/Json/JsonSerializer.h"

template<typename T>
class EditorValueField {
public:
	EditorValueField(const std::string& name_) :
		showObject(name_, &value) {
	}
	~EditorValueField() = default;

	__CLASS_DEFAULT_ALL(EditorValueField<T>)

public:
	void show_gui() {
		std::bitset<2> result = showObject.show_gui();
		if (result == 0b01) {
			EditorValueChangeCommandHandler::GenCommand<T>(value);
		}
		else if (result == 0b10) {
			EditorValueChangeCommandHandler::End();
		}
	}

	friend struct nlohmann::adl_serializer<EditorValueField<T>>;

public:
	void set_weak(const T& value_) {
		value = value_;
	}
	void set(const T& value_) {
		EditorValueChangeCommandHandler::GenCommand<T>(value);
		value = value_;
		EditorValueChangeCommandHandler::End();
	}
	T& get() { return value; };
	T copy() const { return value; }
	std::string_view label() const {
		return showObject.get_name();
	}

private:
	T value;
	ValueEditor::show_object<T> showObject;
};

namespace nlohmann {

template<typename T>
	requires std::copyable<T>
struct adl_serializer<EditorValueField<T>> {
	static inline void to_json(nlohmann::json& j, const EditorValueField<T>& p) {
		j[p.label()] = p.value;
	}

	static inline void from_json(const nlohmann::json& j, EditorValueField<T>& p) {
		if (j.contains(p.label())) {
			j[p.label()].get_to(p.value);
		}
	}
};

}

template<>
class EditorValueField<Transform3D> {
public:
	EditorValueField(const std::string& name_) :
		gui_label(name_),
		scaleShowObject("Scale", &value.get_scale()),
		rotateShowObject("Rotate", &value.get_quaternion()),
		translateShowObject("Translate", &value.get_translate()) {
	};
	~EditorValueField() = default;

	__CLASS_DEFAULT_ALL(EditorValueField<Transform3D>)

public:
	void show_gui() {
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode(std::format("{}##{:}", gui_label, (void*)this).c_str())) {
			{
				std::bitset<2> result = 0;
				// ---------- Scale ----------
				Vector3& scale = value.get_scale();
				// リセットボタン
				if (ImGui::Button("R##Scale")) {
					scale = CVector3::BASIS;
				}
				result.set(ImGui::IsItemDeactivated(), 1);
				result.set(ImGui::IsItemActivated(), 0);
				ImGui::SameLine();
				ImGui::SetNextItemWidth(150);
				result |= scaleShowObject.show_gui();
				if (result == 0b01) {
					EditorValueChangeCommandHandler::GenCommand<Vector3>(scale);
				}
				else if (result == 0b10) {
					EditorValueChangeCommandHandler::End();
				}
			}

			{
				std::bitset<2> result = 0;
				// ---------- Rotate ----------
				Quaternion& rotate = value.get_quaternion();
				// リセットボタン
				if (ImGui::Button("R##Rotate")) {
					rotate = CQuaternion::IDENTITY;
				}
				result.set(ImGui::IsItemDeactivated(), 1);
				result.set(ImGui::IsItemActivated(), 0);

				result |= rotateShowObject.show_gui();
				if (result == 0b01) {
					EditorValueChangeCommandHandler::GenCommand<Quaternion>(rotate);
				}
				else if (result == 0b10) {
					EditorValueChangeCommandHandler::End();
				}
			}

			{
				std::bitset<2> result = 0;
				// ---------- Translate ----------
				Vector3& translate = value.get_translate();
				// リセットボタン
				if (ImGui::Button("R##Translate")) {
					translate = CVector3::ZERO;
				}
				result.set(ImGui::IsItemDeactivated(), 1);
				result.set(ImGui::IsItemActivated(), 0);
				ImGui::SameLine();
				ImGui::SetNextItemWidth(150);
				result |= translateShowObject.show_gui();
				if (result == 0b01) {
					EditorValueChangeCommandHandler::GenCommand<Vector3>(translate);
				}
				else if (result == 0b10) {
					EditorValueChangeCommandHandler::End();
				}
			}

			ImGui::TreePop();
		}
	}

	Transform3D& get() { return value; };
	void set_weak(const Transform3D& value_) {
		value.copy(value_);
	}
	void set(const Transform3D& value_) {
		EditorCommandInvoker::Execute(std::make_unique<EditorCommandScopeBegin>());

		EditorValueChangeCommandHandler::GenCommand<Vector3>(value.get_scale());
		value.set_scale(value_.get_scale());
		EditorValueChangeCommandHandler::End();
		EditorValueChangeCommandHandler::GenCommand<Quaternion>(value.get_quaternion());
		value.set_quaternion(value_.get_quaternion());
		EditorValueChangeCommandHandler::End();
		EditorValueChangeCommandHandler::GenCommand<Vector3>(value.get_translate());
		value.set_translate(value_.get_translate());
		EditorValueChangeCommandHandler::End();

		EditorCommandInvoker::Execute(std::make_unique<EditorCommandScopeEnd>());
	};
	Transform3D copy() const {
		Transform3D copy;
		copy.copy(value);
		return copy;
	}
	std::string_view label() const {
		return gui_label;
	}

	friend struct nlohmann::adl_serializer<EditorValueField<Transform3D>>;

private:
	std::string gui_label;

	Transform3D value;

	ValueEditor::show_object<Vector3> scaleShowObject;
	ValueEditor::show_object<Quaternion> rotateShowObject;
	ValueEditor::show_object<Vector3> translateShowObject;
};

namespace nlohmann {

template<>
struct adl_serializer<EditorValueField<Transform3D>> {
	static inline void to_json(nlohmann::json& j, const EditorValueField<Transform3D>& p) {
		j[p.label()] = p.value;
	}

	static inline void from_json(const nlohmann::json& j, EditorValueField<Transform3D>& p) {
		if (j.contains(p.label())) {
			j[p.label()].get_to(p.value);
		}
	}
};

}

template<>
class EditorValueField<Transform2D> {
public:
	EditorValueField(const std::string& name_) :
		gui_label(name_),
		scaleShowObject("Scale", &value.get_scale()),
		rotateShowObject("Rotate", &value.get_rotate()),
		translateShowObject("Translate", &value.get_translate()) {
	};
	~EditorValueField() = default;

	__CLASS_DEFAULT_ALL(EditorValueField<Transform2D>)

public:
	void show_gui() {
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode(std::format("{}##{:}", gui_label, (void*)this).c_str())) {
			{
				std::bitset<2> result = 0;
				// ---------- Scale ----------
				Vector2& scale = value.get_scale();
				// リセットボタン
				if (ImGui::Button("R##Scale")) {
					scale = CVector2::BASIS;
				}
				result.set(ImGui::IsItemDeactivated(), 1);
				result.set(ImGui::IsItemActivated(), 0);
				ImGui::SameLine();
				ImGui::SetNextItemWidth(150);
				result |= scaleShowObject.show_gui();
				if (result == 0b01) {
					EditorValueChangeCommandHandler::GenCommand<Vector2>(scale);
				}
				else if (result == 0b10) {
					EditorValueChangeCommandHandler::End();
				}
			}

			{
				std::bitset<2> result = 0;
				// ---------- Rotate ----------
				r32& rotate = value.get_rotate();
				// リセットボタン
				if (ImGui::Button("R##Rotate")) {
					rotate = 0;
				}
				result.set(ImGui::IsItemDeactivated(), 1);
				result.set(ImGui::IsItemActivated(), 0);
				ImGui::SameLine();
				ImGui::SetNextItemWidth(150);
				result |= rotateShowObject.show_gui();
				if (result == 0b01) {
					EditorValueChangeCommandHandler::GenCommand<r32>(rotate);
				}
				else if (result == 0b10) {
					EditorValueChangeCommandHandler::End();
				}
			}

			{
				std::bitset<2> result = 0;
				// ---------- Translate ----------
				Vector2& translate = value.get_translate();
				// リセットボタン
				if (ImGui::Button("R##Translate")) {
					translate = CVector2::ZERO;
				}
				result.set(ImGui::IsItemDeactivated(), 1);
				result.set(ImGui::IsItemActivated(), 0);
				ImGui::SameLine();
				ImGui::SetNextItemWidth(150);
				result |= translateShowObject.show_gui();
				if (result == 0b01) {
					EditorValueChangeCommandHandler::GenCommand<Vector2>(translate);
				}
				else if (result == 0b10) {
					EditorValueChangeCommandHandler::End();
				}
			}

			ImGui::TreePop();
		}
	}

	Transform2D& get() { return value; };
	void set_weak(const Transform2D& value_) {
		value.copy(value_);
	}
	void set(const Transform2D& value_) {
		EditorCommandInvoker::Execute(std::make_unique<EditorCommandScopeBegin>());

		EditorValueChangeCommandHandler::GenCommand<Vector2>(value.get_scale());
		value.set_scale(value_.get_scale());
		EditorValueChangeCommandHandler::End();
		EditorValueChangeCommandHandler::GenCommand<r32>(value.get_rotate());
		value.set_rotate(value_.get_rotate());
		EditorValueChangeCommandHandler::End();
		EditorValueChangeCommandHandler::GenCommand<Vector2>(value.get_translate());
		value.set_translate(value_.get_translate());
		EditorValueChangeCommandHandler::End();

		EditorCommandInvoker::Execute(std::make_unique<EditorCommandScopeEnd>());
	};
	Transform2D copy() const {
		Transform2D copy;
		copy.copy(value);
		return copy;
	}
	std::string_view label() const {
		return gui_label;
	}

	friend struct nlohmann::adl_serializer<EditorValueField<Transform2D>>;

private:
	std::string gui_label;

	Transform2D value;

	ValueEditor::show_object<Vector2> scaleShowObject;
	ValueEditor::show_object<r32> rotateShowObject;
	ValueEditor::show_object<Vector2> translateShowObject;
};

namespace nlohmann {

template<>
struct adl_serializer<EditorValueField<Transform2D>> {
	static inline void to_json(nlohmann::json& j, const EditorValueField<Transform2D>& p) {
		j[p.label()] = p.value;
	}

	static inline void from_json(const nlohmann::json& j, EditorValueField<Transform2D>& p) {
		if (j.contains(p.label())) {
			j[p.label()].get_to(p.value);
		}
	}
};

}

#endif // DEBUG_FEATURES_ENABLE
