#ifdef DEBUG_FEATURES_ENABLE

#include "FolderObject.h"

#include "RemoteWorldInstance.h"

#include <imgui.h>
#include <imgui_stdlib.h>

void FolderObject::initialize() {
}

void FolderObject::draw_inspector() {
	ImGui::InputText("FolderName", &hierarchyName);
}

void FolderObject::draw_hierarchy(Reference<IRemoteObject>& select) {
	bool isSelected = this == select.ptr();

	int flags =
		ImGuiTreeNodeFlags_SpanFullWidth |
		ImGuiTreeNodeFlags_NoTreePushOnOpen |
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
		select = this;
	}

	if (isOpen) {
		for (auto& child : childrenFolders) {
			child->draw_hierarchy(select);
		}
		for (auto& child : childrenInstances) {
			child->draw_hierarchy(select);
		}
	}
}

#endif // DEBUG_FEATURES_ENABLE
