#ifdef DEBUG_FEATURES_ENABLE

#include "EditorGizmo.h"

#include <imgui.h>

#include "./EditorSelectObject.h"

#include <Library/Math/Affine.h>

#include "./RemoteObject/IRemoteObject.h"
#include "Command/EditorValueChangeCommandHandler.h"
#include "Engine/Module/World/Camera/Camera3D.h"
#include "RemoteObject/RemoteWorldObject.h"

EditorGizmo::EditorGizmo() = default;
EditorGizmo::~EditorGizmo() = default;

void EditorGizmo::begin_frame(const Vector2& origin, const Vector2& size) {
	ImGuizmo::SetRect(origin.x, origin.y, size.x, size.y);
}

void EditorGizmo::draw_gizmo(Reference<EditorSelectObject> select, Reference<const EditorWorldView> world) {
	if (!select || !world) {
		return;
	}

	// get select
	auto& item = select->get_item();

	// null check
	if (!item.object || !item.transform) {
		return;
	}

	// RootWorld check
	{
		bool check = false;
		Reference<IRemoteObject> remoteObject = item.object;
		while (remoteObject) {
			if (remoteObject == world->remote_world()) {
				check = true;
				break;
			}
			remoteObject = remoteObject->get_parent();
		}
		if (!check) {
			// RootWorldが違う
			return;
		}
	}

	// SetCamera
	{
		Reference<const Camera3D> camera = world->get_camera();
		view = camera->view_affine().to_matrix();
		proj = camera->proj_matrix();
	}

	// to matrix
	Matrix4x4 matrix = Affine::FromTransform3D(*item.transform).to_matrix();

	// manipulate
	gizmoState <<= 1;
	ImGuizmo::Manipulate(&view[0][0], &proj[0][0], operation, mode, &matrix[0][0]);
	gizmoState.set(0, ImGuizmo::IsUsing());
	if (gizmoState == 0b01) {
		switch (operation) {
		case ImGuizmo::OPERATION::SCALEU:
			EditorValueChangeCommandHandler::GenCommand<Vector3>(item.transform->get_scale());
			break;
		case ImGuizmo::OPERATION::ROTATE:
			EditorValueChangeCommandHandler::GenCommand<Quaternion>(item.transform->get_quaternion());
			break;
		case ImGuizmo::OPERATION::TRANSLATE:
			EditorValueChangeCommandHandler::GenCommand<Vector3>(item.transform->get_translate());
			break;
		default:
			break;
		}
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

		ImGui::EndCombo();
	}
}

#endif // DEBUG_FEATURES_ENABLE
