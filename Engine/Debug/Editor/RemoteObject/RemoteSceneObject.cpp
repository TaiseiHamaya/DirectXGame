#ifdef DEBUG_FEATURES_ENABLE

#include "RemoteSceneObject.h"

#include <format>

#include <imgui.h>
#include <imgui_stdlib.h>

#include "../Command/EditorCommandInvoker.h"
#include "../Command/EditorSelectCommand.h"
#include "IRemoteObject.h"
#include "RemoteWorldObject.h"

RemoteSceneObject::RemoteSceneObject() = default;
RemoteSceneObject::~RemoteSceneObject() = default;

void RemoteSceneObject::draw_inspector() {
	ImGui::InputText("SceneName", &hierarchyName);
}

void RemoteSceneObject::draw_hierarchy(Reference<const EditorSelectObject> select) {
	bool isSelected = select->is_selected(this);

	int flags =
		ImGuiTreeNodeFlags_SpanFullWidth |
		ImGuiTreeNodeFlags_OpenOnArrow | // 矢印で開く
		ImGuiTreeNodeFlags_OpenOnDoubleClick; // ダブルクリックで開く
	if (isSelected) {
		flags |= ImGuiTreeNodeFlags_Selected; // 選択時は選択状態にする
	}
	if (isOpen) {
		flags |= ImGuiTreeNodeFlags_DefaultOpen;
	}
	isOpen = ImGui::TreeNodeEx(std::format("{}##{}", hierarchyName, (void*)this).c_str(), flags);

	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
		EditorCommandInvoker::Execute(
			std::make_unique<EditorSelectCommand>(this)
		);
	}

	if (isOpen) {
		for (auto& world : remoteWorlds) {
			world->draw_hierarchy(select);
		}
		ImGui::TreePop();
	}
}

#endif // DEBUG_FEATURES_ENABLE
