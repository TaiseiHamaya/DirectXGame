#ifdef DEBUG_FEATURES_ENABLE

#include "RemoteWorldObject.h"

#include <format>

#include <imgui.h>
#include <imgui_stdlib.h>

#include "../Command/EditorCommandInvoker.h"
#include "../Command/EditorSelectCommand.h"

RemoteWorldObject::RemoteWorldObject() = default;
RemoteWorldObject::~RemoteWorldObject() = default;

void RemoteWorldObject::draw_inspector() {
	ImGui::InputText("World", &hierarchyName);
}

void RemoteWorldObject::draw_hierarchy(Reference<const EditorSelectObject> select) {
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
		for (auto& child : children) {
			if (!child) {
				continue;
			}
			child->draw_hierarchy(select);
		}
		ImGui::TreePop();
	}
}

#endif // DEBUG_FEATURES_ENABLE
