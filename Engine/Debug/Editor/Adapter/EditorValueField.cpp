#ifdef DEBUG_FEATURES_ENABLE

#include "EditorValueField.h"

void Transform3DShowGuiBody(const std::string& gui_label, Transform3D& transform) {
	const ValueEditor::show_object<Vector3> scaleObj{ "Scale" };
	const ValueEditor::show_object<Quaternion> rotateObj{ "Rotate" };
	const ValueEditor::show_object<Vector3> translateObj{ "Translate" };
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNodeEx(gui_label.c_str(), ImGuiTreeNodeFlags_SpanAllColumns)) {
		{
			std::bitset<2> result = 0;
			// ---------- Scale ----------
			Vector3& scale = transform.get_scale();
			// リセットボタン
			if (ImGui::Button("\ue5d5##Scale")) {
				scale = CVector3::BASIS;
			}
			result.set(ImGui::IsItemDeactivated(), 1);
			result.set(ImGui::IsItemActivated(), 0);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(150);
			result |= scaleObj.show_gui(scale);
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
			Quaternion& rotate = transform.get_quaternion();
			// リセットボタン
			if (ImGui::Button("\ue5d5##Rotate")) {
				rotate = CQuaternion::IDENTITY;
			}
			result.set(ImGui::IsItemDeactivated(), 1);
			result.set(ImGui::IsItemActivated(), 0);

			ImGui::SameLine();
			ImGui::Indent(29.f);
			result |= rotateObj.show_gui(rotate);
			ImGui::Unindent(29.f);
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
			Vector3& translate = transform.get_translate();
			// リセットボタン
			if (ImGui::Button("\ue5d5##Translate")) {
				translate = CVector3::ZERO;
			}
			result.set(ImGui::IsItemDeactivated(), 1);
			result.set(ImGui::IsItemActivated(), 0);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(150);
			result |= translateObj.show_gui(translate);
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

void Transform2DShowGuiBody(const std::string& gui_label, Transform2D& transform) {
	const ValueEditor::show_object<Vector2> scaleObj{ "Scale" };
	const ValueEditor::show_object<r32> rotateObj{ "Rotate" };
	const ValueEditor::show_object<Vector2> translateObj{ "Translate" };
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNodeEx(gui_label.c_str(), ImGuiTreeNodeFlags_SpanAllColumns)) {
		{
			std::bitset<2> result = 0;
			// ---------- Scale ----------
			Vector2& scale = transform.get_scale();
			// リセットボタン
			if (ImGui::Button("\ue5d5##Scale")) {
				scale = CVector2::BASIS;
			}
			result.set(ImGui::IsItemDeactivated(), 1);
			result.set(ImGui::IsItemActivated(), 0);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(150);
			result |= scaleObj.show_gui(scale);
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
			r32& rotate = transform.get_rotate();
			// リセットボタン
			if (ImGui::Button("\ue5d5##Rotate")) {
				rotate = 0;
			}
			result.set(ImGui::IsItemDeactivated(), 1);
			result.set(ImGui::IsItemActivated(), 0);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(150);
			result |= rotateObj.show_gui(rotate);
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
			Vector2& translate = transform.get_translate();
			// リセットボタン
			if (ImGui::Button("\ue5d5##Translate")) {
				translate = CVector2::ZERO;
			}
			result.set(ImGui::IsItemDeactivated(), 1);
			result.set(ImGui::IsItemActivated(), 0);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(150);
			result |= translateObj.show_gui(translate);
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

#endif // DEBUG_FEATURES_ENABLE
