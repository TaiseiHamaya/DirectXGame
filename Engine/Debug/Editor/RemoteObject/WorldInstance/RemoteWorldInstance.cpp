#ifdef DEBUG_FEATURES_ENABLE

#include "RemoteWorldInstance.h"

#include <imgui_stdlib.h>

RemoteWorldInstance::RemoteWorldInstance() = default;
RemoteWorldInstance::~RemoteWorldInstance() = default;

void RemoteWorldInstance::draw_inspector() {
	ImGui::Text("Type : WorldInstance");

	ImGui::InputText("Name", &hierarchyName);
	ImGui::Separator();

	transform.debug_gui("Local transform");
}

void RemoteWorldInstance::draw_hierarchy(Reference<IRemoteObject>& select) {
	bool isSelected = this == select.ptr();
	// 子がいる場合
	if (!children.empty()) {
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

		// こうすると選択できるらしい
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
			select = this;
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
			select = this;
		}
	}
}

#endif // DEBUG_FEATURES_ENABLE
