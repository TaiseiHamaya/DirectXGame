#ifdef DEBUG_FEATURES_ENABLE

#include "RemoteWorldInstance.h"

#include <imgui_stdlib.h>

#include "../../Command/EditorCommandInvoker.h"
#include "../../Command/EditorObjectMoveCommand.h"
#include "../../Command/EditorSelectCommand.h"

RemoteWorldInstance::RemoteWorldInstance() = default;
RemoteWorldInstance::~RemoteWorldInstance() = default;

void RemoteWorldInstance::draw_inspector() {
	ImGui::Text("Type : WorldInstance");

	ImGui::InputText("Name", &hierarchyName);
	ImGui::Separator();

	bool isMove = transform.debug_gui("Local transform");
	if (isMove && !moveCommand) {
		moveCommand = std::make_unique<EditorObjectMoveCommand>(transform);
	}
	else if (!isMove && moveCommand) {
		moveCommand->preprocess();
		EditorCommandInvoker::Execute(std::move(moveCommand));
	}
}

void RemoteWorldInstance::draw_hierarchy(Reference<const EditorSelectObject> select) {
	bool isSelected = select->is_selected(this);
	// 子がいる場合
	if (!children.empty()) {
		int flags =
			ImGuiTreeNodeFlags_DrawLinesToNodes |
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

		// こうすると選択できるらしい
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
			EditorCommandInvoker::Execute(
				std::make_unique<EditorSelectCommand>(this, transform)
			);
		}

		if (isOpen) {
			for (auto& child : children) {
				child->draw_hierarchy(select);
			}
			ImGui::TreePop();
		}
	}
	// 子がいない場合
	else {
		if (ImGui::Selectable(std::format("{}##{}", hierarchyName, (void*)this).c_str(), isSelected)) {
			EditorCommandInvoker::Execute(
				std::make_unique<EditorSelectCommand>(this, transform)
			);
		}
	}
}

#endif // DEBUG_FEATURES_ENABLE
