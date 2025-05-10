#ifdef DEBUG_FEATURES_ENABLE

#include "EditorMain.h"

#include "Engine/Application/EngineSettings.h"

#include <imgui.h>

void EditorMain::Initialize() {
	EditorMain& instance = GetInstance();
	instance.isActiveEditor = true;
	instance.sceneView.initialize(true);
}

void EditorMain::DrawBase() {
	EditorMain& instance = GetInstance();
	if (!instance.isActiveEditor) {
		return;
	}

	instance.set_imgui_command();
}

void EditorMain::Draw() {
	EditorMain& instance = GetInstance();
	if (!instance.isActiveEditor) {
		return;
	}

	instance.sceneView.draw();
}

void EditorMain::Finalize() {
}

bool EditorMain::IsHoverEditorWindow() {
	EditorMain& instance = GetInstance();
	return instance.sceneView.is_hovered_window();
}

void EditorMain::set_imgui_command() {
	EditorMain& instance = GetInstance();

	ImGui::SetNextWindowPos({ 0,0 });
	ImGui::SetNextWindowSize({ EngineSettings::CLIENT_SIZE.x, EngineSettings::CLIENT_SIZE.y });
	i32 flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
	ImGui::Begin("EditorMain", nullptr, flags);
	ImGuiID dockSpaceId = ImGui::GetID("EditorMain");
	ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::End();
}

#endif // DEBUG_FEATURES_ENABLE
