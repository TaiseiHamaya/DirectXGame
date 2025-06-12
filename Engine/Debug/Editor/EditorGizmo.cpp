#include "EditorGizmo.h"

#include <imgui.h>

#include "./EditorSelectObject.h"

#include <Library/Math/Affine.h>

#include "./RemoteObject/IRemoteObject.h"
#include "Command/EditorValueChangeCommandHandler.h"
#include "Engine/Module/World/Camera/Camera3D.h"

EditorGizmo::EditorGizmo() = default;
EditorGizmo::~EditorGizmo() = default;

void EditorGizmo::begin_frame(Reference<Camera3D> camera, const Vector2& origin, const Vector2& size) {
	ImGuizmo::SetRect(origin.x, origin.y, size.x, size.y);
	if (camera) {
		view = camera->debug_view_affine().to_matrix();
		proj = camera->debug_proj_matrix();
	}
}

void EditorGizmo::draw_gizmo(Reference<EditorSelectObject> select) {
	if (!select) {
		return;
	}

	// get select
	auto& item = select->get_item();

	// null check
	if (!item.object || !item.transform) {
		return;
	}

	// to matrix
	Matrix4x4 matrix = Affine::FromTransform3D(*item.transform).to_matrix();

	// manipulate
	gizmoState <<= 1;
	ImGuizmo::Manipulate(&view[0][0], &proj[0][0], operation, mode, &matrix[0][0]);
	gizmoState.set(0, ImGuizmo::IsUsing());
	if (gizmoState == 0b01) {
		EditorValueChangeCommandHandler::GenCommand<Transform3D, &Transform3D::copy>(item.transform);
	}
	else if (gizmoState == 0b10) {
		EditorValueChangeCommandHandler::End();
	}

	// to affine
	Affine affine = Affine::FromMatrix(matrix);

	// decompose
	item.transform->set_scale(affine.get_basis().to_scale());
	item.transform->set_quaternion(affine.get_basis().to_quaternion());
	item.transform->set_translate(affine.get_origin());
}

void EditorGizmo::scene_header() {
	ImGui::SetNextItemWidth(100);
	std::string modeComboLabel = mode == ImGuizmo::MODE::WORLD ? "World" : "Local";
	if (ImGui::BeginCombo(std::format("##Mode{}", (void*)this).c_str(), modeComboLabel.c_str())) {
		if (ImGui::Selectable("Local", mode == ImGuizmo::MODE::LOCAL)) {
			mode = ImGuizmo::MODE::LOCAL;
		}
		if (ImGui::Selectable("World", mode == ImGuizmo::MODE::WORLD)) {
			mode = ImGuizmo::MODE::WORLD;
		}

		ImGui::EndCombo();
	}

	ImGui::SameLine();

	ImGui::SetNextItemWidth(100);
	std::string operationComboLabel;
	switch (operation) {
	case ImGuizmo::OPERATION::SCALEU:
		operationComboLabel = "Scale";
		break;
	case ImGuizmo::OPERATION::ROTATE:
		operationComboLabel = "Rotate";
		break;
	case ImGuizmo::OPERATION::TRANSLATE:
		operationComboLabel = "Translate";
		break;
	case ImGuizmo::OPERATION::UNIVERSAL:
		operationComboLabel = "Universal";
		break;
	}
	if (ImGui::BeginCombo(std::format("##Operation{}", (void*)this).c_str(), operationComboLabel.c_str())) {
		if (ImGui::Selectable("Scale", operation == ImGuizmo::OPERATION::SCALEU)) {
			operation = ImGuizmo::OPERATION::SCALEU;
		}
		if (ImGui::Selectable("Rotate", operation == ImGuizmo::OPERATION::ROTATE)) {
			operation = ImGuizmo::OPERATION::ROTATE;
		}
		if (ImGui::Selectable("Translate", operation == ImGuizmo::OPERATION::TRANSLATE)) {
			operation = ImGuizmo::OPERATION::TRANSLATE;
		}
		if (ImGui::Selectable("Universal", operation == ImGuizmo::OPERATION::UNIVERSAL)) {
			operation = ImGuizmo::OPERATION::UNIVERSAL;
		}

		ImGui::EndCombo();
	}
}
