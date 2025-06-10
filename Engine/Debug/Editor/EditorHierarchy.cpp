#ifdef DEBUG_FEATURES_ENABLE

#include "EditorHierarchy.h"

#include <imgui.h>
#include <imgui_stdlib.h>

#include "Adapter/EditorSceneSerializer.h"
#include "Engine/Runtime/Scene/SceneManager.h"

#include <Engine/Assets/Json/JsonAsset.h>

void EditorHierarchy::load(std::filesystem::path file) {
	savedTrigger = false;
	isActive = true;

	JsonAsset json{ file };
	scene = EditorSceneSerializer::CreateRemoteScene(json.try_emplace<nlohmann::json>("Scene"));

	Reference<BaseScene> currentScene = SceneManager::GetCurrentScene();
}

void EditorHierarchy::draw() {
	if (!isActive) {
		return;
	}

	int flags = 0;
	ImGui::Begin("Hierarchy", &isActive, flags);
	savedTrigger = false;

	// 検索ボックス
	size_t beforSize = searchString.size();
	ImGui::InputText("##HierarchySearch", &searchString); ImGui::SameLine();
	if (ImGui::Button("\ue5cd")) {
		searchString.clear();
	}

	ImVec2 tempPos = ImGui::GetCursorPos();

	if (searchString.empty()) {
		ImGui::SetCursorPos({ 16, 30 });
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(100, 100, 100, 255));
		ImGui::Text("Search");
		ImGui::PopStyleColor();
	}

	ImGui::SetCursorPos(tempPos);
	scene->draw_hierarchy(select);

	// 右クリックメニュー
	if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
		ImGui::OpenPopup("HierarchyMenu");
	}

	if (ImGui::BeginPopup("HierarchyMenu")) {
		// Instance作成
		if (ImGui::BeginMenu("CreateInstance")) {
			ImGui::InputText("##MenuSearch", &menuString); ImGui::SameLine();
			if (ImGui::Button("\ue5cd")) {
				menuString.clear();
			}
			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("CreateWorld")) {

		}

		// Folder作成
		if (ImGui::MenuItem("CreateFolder")) {
			if (select) {
				//select->add_hierarchy();
			}
		}

		ImGui::SetNextItemShortcut(ImGuiKey_S | ImGuiMod_Ctrl, ImGuiInputFlags_RouteFromRootWindow);
		if (ImGui::MenuItem("SaveScene")) {
			savedTrigger = true;
		}

		ImGui::EndPopup();
	}

	ImGui::End();
}

Reference<IRemoteObject> EditorHierarchy::get_select() const {
	return select;
}

#endif // DEBUG_FEATURES_ENABLE
