#ifdef DEBUG_FEATURES_ENABLE

#include "EditorSceneView.h"

#include <imgui.h>

#include "../EditorGizmo.h"

#include "EditorHierarchy.h"
#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/ScreenTexture.h"
#include "Engine/GraphicsAPI/DirectX/DxSwapChain/DxSwapChain.h"

void EditorSceneView::initialize(bool isActive_) {
	isActive = isActive_;
	screenResultTexture.initialize();
}

void EditorSceneView::setup(Reference<EditorGizmo> gizmo_, Reference<const EditorHierarchy> hierarchy_) {
	gizmo = gizmo_;
	hierarchy = hierarchy_;
}

void EditorSceneView::draw_scene() {
	if (worldView.contains(selectWorldObject)) {
		worldView[selectWorldObject].rendering();
	}
	copy_screen();
}

void EditorSceneView::draw() {
	set_imgui_command();
}

bool EditorSceneView::is_hovered_window() {
	return
		(isActive && isHoverWindow) ||
		!ImGui::GetIO().WantCaptureMouse;
}

const Vector2& EditorSceneView::view_origin() const {
	return origin;
}

const Vector2& EditorSceneView::view_size() const {
	return size;
}

Reference<ImDrawList> EditorSceneView::draw_list() const {
	return drawList;
}

void EditorSceneView::check_world(Reference<RemoteWorldObject> worldRef) {
	if (!worldRef) {
		return;
	}
	if (worldView.contains(worldRef)) {
		return;
	}
	// 新規にWorldViewを作成
	auto& view = worldView[worldRef];
	view.initialize();
	view.setup(worldRef);
}

Reference<EditorWorldView> EditorSceneView::get_world_view(Reference<const RemoteWorldObject> worldRef) {
	if (worldView.contains(worldRef)) {
		return worldView[worldRef];
	}
	return nullptr;
}

Reference<EditorWorldView> EditorSceneView::get_current_world_view() {
	if (worldView.empty() || !worldView.contains(selectWorldObject)) {
		return nullptr;
	}
	return worldView[selectWorldObject];
}

void EditorSceneView::copy_screen() {
	auto& command = DxCommand::GetCommandList();
	Reference<ScreenTexture> screen = DxSwapChain::GetWriteBufferTexture();

	screenResultTexture.start_copy_dest();
	screen->start_read();

	command->CopyResource(screenResultTexture.get_resource().Get(), screen->get_resource().Get());
}

void EditorSceneView::set_imgui_command() {
	if (!isActive) {
		return;
	}
	if (isHoverWindow && (ImGui::GetIO().MouseDown[1] || ImGui::GetIO().MouseDown[2])) {
		ImGui::SetNextWindowFocus();
	}

	screenResultTexture.start_read();
	ImGui::Begin("Scene", &isActive, ImGuiWindowFlags_NoScrollbar);

	// Gizmo用ヘッダー描画
	gizmo->scene_header();
	ImGui::Separator();
	ImGui::BeginTabBar("WorldViewTabBar");

	// Guizmoのために必要
	drawList = ImGui::GetWindowDrawList();
	isHoverWindow = ImGui::IsWindowHovered();

	// 各WorldViewをImGuiに描画
	auto& worldList = hierarchy->world_list();
	for (u32 i = 0; i < worldList.size(); ++i) {
		auto& world = worldList[i];
		if (worldView.contains(world)) {
			auto [result, pos, size_] = worldView.at(world).draw_editor(screenResultTexture);
			if (result) {
				selectWorldObject = world;
				origin = pos;
				size = size_;
			}
		}
	}

	ImGui::EndTabBar();
	ImGui::End();
}

#endif // DEBUG_FEATURES_ENABLE
