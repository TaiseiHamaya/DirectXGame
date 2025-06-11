#ifdef DEBUG_FEATURES_ENABLE

#include "EditorMain.h"

#include "Engine/Application/EngineSettings.h"
#include "Engine/Assets/Json/JsonAsset.h"

#include <imgui.h>

void EditorMain::Initialize() {
	EditorMain& instance = GetInstance();
	instance.isActiveEditor = true;
	instance.sceneView.initialize(true);
	instance.inspector.initialize();

	instance.input.initialize({ KeyID::F5 });
}

void EditorMain::Start() {
	EditorMain& instance = GetInstance();
	JsonAsset json;
	json.load("./Game/DebugData/Editor.json");
	std::string sceneFileName = json.try_emplace<std::string>("LastLoadedScene");
	instance.hierarchy.load(std::format("./Game/Core/Scene/{}.json", sceneFileName));
	instance.inspector.start(instance.hierarchy);
}

void EditorMain::DrawBase() {
	EditorMain& instance = GetInstance();

	instance.input.update();
	if (instance.input.trigger(KeyID::F5)) {
		instance.isActiveEditor ^= 1;
	}

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
	instance.hierarchy.draw();
	instance.inspector.draw();
	if (instance.sceneView.is_active()) {
		instance.gizmo.draw_gizmo(instance.hierarchy.get_select());
	}
}

void EditorMain::Finalize() {
}

bool EditorMain::IsHoverEditorWindow() {
	EditorMain& instance = GetInstance();
	return instance.sceneView.is_hovered_window();
}

void EditorMain::SetCamera(Reference<Camera3D> camera) {
	EditorMain& instance = GetInstance();

	instance.gizmo.begin_frame(camera, instance.sceneView.view_origin(), instance.sceneView.view_size());
}

void EditorMain::set_imgui_command() {
	EditorMain& instance = GetInstance();

	i32 flags =
		ImGuiWindowFlags_MenuBar | // メニューバーを表示
		ImGuiWindowFlags_NoDocking | // ドッキングしない
		ImGuiWindowFlags_NoTitleBar | // タイトルバーなし
		ImGuiWindowFlags_NoResize | // リサイズしない
		ImGuiWindowFlags_NoScrollbar | // スクロールバーなし
		ImGuiWindowFlags_NoBringToFrontOnFocus; // 最背面

	ImGui::SetNextWindowPos({ 0,0 });
	ImGui::SetNextWindowSize({ EngineSettings::CLIENT_SIZE.x, EngineSettings::CLIENT_SIZE.y });

	ImGui::Begin("EditorMain", nullptr, flags);
	// メニューバーの表示
	if (ImGui::BeginMenuBar()) {
		// Windowメニュー
		if (ImGui::BeginMenu("Window")) {
			sceneView.draw_menu("Scene");
			hierarchy.draw_menu("Hierarchy");
			inspector.draw_menu("Inspector");
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// メインのドックスペースを追加
	ImGuiID dockSpaceId = ImGui::GetID("EditorMain");
	ImGui::SetCursorScreenPos({ 0, 19 });
	ImGui::DockSpace(dockSpaceId, ImVec2(EngineSettings::CLIENT_SIZE.x, EngineSettings::CLIENT_SIZE.y - 19), ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::End();
}

#endif // DEBUG_FEATURES_ENABLE
