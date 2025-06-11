#include "EditorGizmo.h"

#include <imgui.h>

#include <Library/Math/Affine.h>

#include "Engine/Module/World/Camera/Camera3D.h"
#include <imgui_internal.h>

void EditorGizmo::begin_frame(Reference<Camera3D> camera, const Vector2& origin, const Vector2& size) {
	ImGuizmo::SetRect(origin.x, origin.y, size.x, size.y);
	if (camera) {
		view = camera->debug_view_affine().to_matrix();
		proj = camera->debug_proj_matrix();
	}
}

void EditorGizmo::draw_gizmo(Reference<IRemoteObject> object) {
	if (!object) {
		return;
	}
	ImGuiContext& g = *ImGui::GetCurrentContext();
	ImGuiWindow* window = ImGui::FindWindowByName("gizmo");
	if (g.HoveredWindow == window)   // Mouse hovering drawlist window
		Information("g.HoveredWindow == window");
	//if (gContext.mAlternativeWindow != nullptr && g.HoveredWindow == gContext.mAlternativeWindow)
	//	Information("g.HoveredWindow == window");
	if (g.HoveredWindow != NULL)     // Any other window is hovered
		Information("g.HoveredWindow != NULL");
	if (ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max, false))   // Hovering drawlist window rect, while no other window is hovered (for _NoInputs windows)
		Information("ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max, false)");
	object->draw_gizmo();

	Matrix4x4 mat = CMatrix4x4::IDENTITY;
	
	ImGuizmo::Manipulate(&view[0][0], &proj[0][0], operation, ImGuizmo::WORLD, &mat[0][0]);

	mat = mat;
	Affine affine = Affine::FromMatrix(mat);
	Information("{}", ImGuizmo::IsOver());
}
