#ifdef DEBUG_FEATURES_ENABLE

#include "EditorHierarchy.h"

#include <imgui.h>
#include <imgui_stdlib.h>

#include "../Adapter/EditorSceneSerializer.h"
#include "EditorSceneView.h"

#include "../Command/EditorCommandInvoker.h"
#include "../Command/EditorCreateObjectCommand.h"
#include "../Command/EditorDeleteObjectCommand.h"

#include "../RemoteObject/FolderObject.h"
#include "../RemoteObject/WorldInstance/Camera/RemoteCamera3dInstance.h"
#include "../RemoteObject/WorldInstance/Mesh/RemoteSkinningMeshInstance.h"
#include "../RemoteObject/WorldInstance/Mesh/RemoteStaticMeshInstance.h"
#include "../RemoteObject/WorldInstance/Collider/RemoteAABBColliderInstance.h"
#include "../RemoteObject/WorldInstance/Collider/RemoteSphereColliderInstance.h"
#include "../RemoteObject/WorldInstance/RemoteWorldInstance.h"

#include "Engine/Runtime/Scene/SceneManager.h"

#include <Engine/Assets/Json/JsonAsset.h>

void EditorHierarchy::setup(Reference<EditorSelectObject> select_) {
	select = select_;
}

void EditorHierarchy::finalize() {
	JsonAsset json{ "./Game/DebugData/Editor.json" };
	json.get()["LastLoadedScene"] = scene->name();
	scene.reset();
}

void EditorHierarchy::sync_scene_view(Reference<EditorSceneView> sceneView) {
	if (!sceneView) {
		return;
	}
	for (u32 i = 0; i < scene->world_size(); ++i) {
		scene->sync_world_entry(i, sceneView);
	}
}

void EditorHierarchy::load(std::filesystem::path file) {
	savedTrigger = false;
	isActive = true;

	JsonAsset json{ file };
	scene = EditorSceneSerializer::CreateRemoteScene(json.try_emplace<nlohmann::json>("Scene"));

	Reference<BaseScene> currentScene = SceneManager::GetCurrentScene();
}

nlohmann::json EditorHierarchy::save() const {
	return scene->serialize();
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
	if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && ImGui::IsWindowFocused()) {
		ImGui::OpenPopup("HierarchyMenu");
	}

	if (ImGui::BeginPopup("HierarchyMenu")) {
		//Instance作成
		if (ImGui::BeginMenu("CreateInstance")) {
			ImGui::InputText("##MenuSearch", &menuString); ImGui::SameLine();
			if (ImGui::Button("\ue5cd")) {
				menuString.clear();
			}
			if (ImGui::MenuItem("WorldInstance")) {
				if (select->get_item().object) {
					EditorCommandInvoker::Execute(
						std::make_unique<EditorCreateObjectCommand>(
							select->get_item().object,
							std::make_unique<RemoteWorldInstance>()
						)
					);
				}
			}
			if (ImGui::MenuItem("StaticMeshInstance")) {
				if (select->get_item().object) {
					EditorCommandInvoker::Execute(
						std::make_unique<EditorCreateObjectCommand>(
							select->get_item().object,
							std::make_unique<RemoteStaticMeshInstance>()
						)
					);
				}
			}
			if (ImGui::MenuItem("SkinningMeshInstance")) {
				if (select->get_item().object) {
					EditorCommandInvoker::Execute(
						std::make_unique<EditorCreateObjectCommand>(
							select->get_item().object,
							std::make_unique<RemoteSkinningMeshInstance>()
						)
					);
				}
			}
			if (ImGui::MenuItem("Camera3D")) {
				if (select->get_item().object) {
					EditorCommandInvoker::Execute(
						std::make_unique<EditorCreateObjectCommand>(
							select->get_item().object,
							std::make_unique<RemoteCamera3dInstance>()
						)
					);
				}
			}
			if (ImGui::MenuItem("AABBColliderInstance")) {
				if (select->get_item().object) {
					//EditorCommandInvoker::Execute(
					//	std::make_unique<EditorCreateObjectCommand>(
					//		select->get_item().object,
					//		std::make_unique<RemoteAABBColliderInstance>()
					//	)
					//);
				}
			}
			if (ImGui::MenuItem("SphereColliderInstance")) {
				if (select->get_item().object) {
					EditorCommandInvoker::Execute(
						std::make_unique<EditorCreateObjectCommand>(
							select->get_item().object,
							std::make_unique<RemoteSphereColliderInstance>()
						)
					);
				}
			}
			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("CreateWorld")) {
			EditorCommandInvoker::Execute(
				std::make_unique<EditorCreateObjectCommand>(
					scene,
					std::make_unique<RemoteWorldObject>()
				)
			);
		}

		// Folder作成
		if (ImGui::MenuItem("CreateFolder")) {
			if (select->get_item().object) {
				EditorCommandInvoker::Execute(
					std::make_unique<EditorCreateObjectCommand>(
						select->get_item().object,
						std::make_unique<FolderObject>()
					)
				);
			}
		}

		if (ImGui::MenuItem("Delete")) {
			if (select->get_item().object && select->get_item().object.ptr() != scene.get()) {
				EditorCommandInvoker::Execute(
					std::make_unique<EditorDeleteObjectCommand>(
						select->get_item().object
					)
				);
				select->set_item(nullptr);
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

std::string EditorHierarchy::current_scene_name() {
	return scene->name();
}

const std::vector<std::unique_ptr<RemoteWorldObject>>& EditorHierarchy::world_list() const {
	return scene->get_remote_worlds();
}

#endif // DEBUG_FEATURES_ENABLE
