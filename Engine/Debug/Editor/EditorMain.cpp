#ifdef DEBUG_FEATURES_ENABLE

#include "EditorMain.h"

#include <fstream>

#include <imgui.h>

#include "./Core/EditorHierarchyDandD.h"
#include "Command/EditorCommandInvoker.h"
#include "Command/EditorCreateObjectCommand.h"
#include "Command/EditorDeleteObjectCommand.h"
#include "Command/EditorSelectCommand.h"
#include "Engine/Application/EngineSettings.h"
#include "Engine/Assets/Json/JsonAsset.h"

#include "./Window/EditorLogWindow.h"

void EditorMain::Initialize() {
	EditorMain& instance = GetInstance();
	instance.isActiveEditor = true;
	instance.sceneView.initialize(true);
	instance.inspector.initialize();
	instance.sceneList.initialize();
	EditorLogWindow::Initialize(true);

	instance.input.initialize({ KeyID::F6, KeyID::LControl, KeyID::LShift, KeyID::Z, KeyID::S });
}

void EditorMain::Setup() {
	EditorMain& instance = GetInstance();

	EditorCreateObjectCommand::Setup(instance.deletedPool);
	EditorDeleteObjectCommand::Setup(instance.deletedPool);
	EditorSelectCommand::Setup(instance.selectObject);
	IRemoteObject::Setup(instance.sceneView);
	instance.sceneView.setup(instance.gizmo, instance.hierarchy);
	instance.inspector.setup(instance.selectObject);

	JsonAsset json;
	json.load("./Game/DebugData/Editor.json");
	std::string sceneFileName = json.try_emplace<std::string>("LastLoadedScene");
	instance.hierarchy.load(std::format("./Game/Core/Scene/{}.json", sceneFileName));
	instance.hierarchy.setup(instance.selectObject, instance.sceneView);
}

void EditorMain::DrawBase() {
	EditorMain& instance = GetInstance();

	if (instance.switchSceneName.has_value()) {
		// シーンビューを未設定に設定
		instance.sceneView.reset_force();
		// シーンのロード
		instance.hierarchy.load(std::format("./Game/Core/Scene/{}.json", instance.switchSceneName.value()));
		instance.hierarchy.setup(instance.selectObject, instance.sceneView);
		// 選択オブジェクトのリセット
		instance.selectObject.set_item(nullptr);
		// コマンドのリセット
		EditorCommandInvoker::ResetHistoryForce();
	}
	instance.switchSceneName = std::nullopt;

	// HierarchyとSceneViewの同期
	instance.gizmo.begin_frame(instance.sceneView.view_origin(), instance.sceneView.view_size());
	instance.sceneView.update();
	instance.hierarchy.update_preview();
	instance.sceneView.draw_scene();

	instance.input.update();
	if (instance.input.trigger(KeyID::F6)) {
		instance.isActiveEditor ^= 1;
	}

	if (!instance.isActiveEditor) {
		return;
	}

	instance.set_imgui_command();

	if (instance.input.trigger(KeyID::Z) && instance.input.press(KeyID::LControl)) {
		if (instance.input.press(KeyID::LShift)) {
			EditorCommandInvoker::Redo();
		}
		else {
			EditorCommandInvoker::Undo();
		}
	}
	if (instance.input.trigger(KeyID::S) && instance.input.press(KeyID::LControl)) {
		instance.sceneList.add_scene(instance.hierarchy.current_scene_name());

		nlohmann::json root;
		root["Scene"] = instance.hierarchy.save();

		std::filesystem::path filePath = std::format("./Game/Core/Scene/{}.json", instance.hierarchy.current_scene_name());
		auto parentPath = filePath.parent_path();
		if (!parentPath.empty() && !std::filesystem::exists(parentPath)) {
			std::filesystem::create_directories(parentPath);
		}

		std::ofstream ofstream{ filePath, std::ios_base::out };
		ofstream << std::setw(1) << std::setfill('\t') << root;
		if (ofstream.fail()) {
			Warning("Failed to save scene file. ({})", filePath.string());
		}
		else {
			Information("Scene file saved. ({})", filePath.string());
		}
		ofstream.close();
	}

	instance.deletedPool.solution_sequence();
}

void EditorMain::Draw() {
	EditorMain& instance = GetInstance();
	if (!instance.isActiveEditor) {
		return;
	}

	instance.sceneView.draw();
	instance.hierarchy.draw();
	instance.inspector.draw();
	EditorLogWindow::Draw();
	if (instance.sceneView.is_active()) {
		ImGuizmo::SetDrawlist(instance.sceneView.draw_list().ptr());
		instance.gizmo.draw_gizmo(instance.selectObject, instance.sceneView.get_current_world_view());
	}

	EditorHierarchyDandD::ExecuteReparent();
}

void EditorMain::Finalize() {
	EditorMain& instance = GetInstance();
	instance.sceneList.finalize();
}

bool EditorMain::IsHoverEditorWindow() {
	EditorMain& instance = GetInstance();
	return instance.sceneView.is_hovered_window();
}

void EditorMain::set_imgui_command() {
	EditorMain& instance = GetInstance();
	// メニューバーの表示
	if (ImGui::BeginMainMenuBar()) {
		// Windowメニュー
		if (ImGui::BeginMenu("Window")) {
			sceneView.draw_menu("Scene");
			hierarchy.draw_menu("Hierarchy");
			inspector.draw_menu("Inspector");
			EditorLogWindow::DrawMenu("Log");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::BeginMenu("Scene")) {
				std::string currentSceneName = hierarchy.current_scene_name();
				if (sceneList.scene_list_gui(currentSceneName)) {
					switchSceneName = currentSceneName;;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	i32 flags =
		ImGuiWindowFlags_MenuBar | // メニューバーを表示
		ImGuiWindowFlags_NoDocking | // ドッキングしない
		ImGuiWindowFlags_NoTitleBar | // タイトルバーなし
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | // リサイズしない
		ImGuiWindowFlags_NoScrollbar | // スクロールバーなし
		ImGuiWindowFlags_NoBringToFrontOnFocus; // 最背面

	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ EngineSettings::CLIENT_SIZE.x, EngineSettings::CLIENT_SIZE.y });

	ImGui::Begin("EditorMain", nullptr, flags);

	// メインのドックスペースを追加
	ImGuiID dockSpaceId = ImGui::GetID("EditorMain");
	ImGui::SetCursorScreenPos({ 0, 19 });
	ImGui::DockSpace(dockSpaceId, ImVec2(EngineSettings::CLIENT_SIZE.x, EngineSettings::CLIENT_SIZE.y - 19), ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::End();
}

#endif // DEBUG_FEATURES_ENABLE
