#ifdef DEBUG_FEATURES_ENABLE

#include "RemoteWorldInstance.h"

#include <imgui_stdlib.h>

#include "../../Command/EditorCommandInvoker.h"
#include "../../Command/EditorSelectCommand.h"
#include "../../Command/EditorValueChangeCommandHandler.h"

#define TRANSFORM3D_SERIALIZER
#include "Engine/Assets/Json/JsonSerializer.h"

RemoteWorldInstance::RemoteWorldInstance() = default;
RemoteWorldInstance::~RemoteWorldInstance() = default;

void RemoteWorldInstance::draw_inspector() {
	ImGui::Text("Type : WorldInstance");

	ImGui::InputText("Name", &hierarchyName);
	ImGui::Separator();

	u32 isMove = transform.debug_gui("Local transform");
	if (isMove == 0b01) {
		EditorValueChangeCommandHandler::GenCommand<Transform3D, &Transform3D::copy>(transform);
	}
	else if (isMove == 0b10) {
		EditorValueChangeCommandHandler::End();
	}
}

void RemoteWorldInstance::draw_hierarchy(Reference<const EditorSelectObject> select) {
	bool isSelected = select->is_selected(this);
	int flags =
		ImGuiTreeNodeFlags_DrawLinesToNodes |
		ImGuiTreeNodeFlags_SpanAllColumns |
		ImGuiTreeNodeFlags_OpenOnArrow | // 矢印で開く
		ImGuiTreeNodeFlags_OpenOnDoubleClick; // ダブルクリックで開く
	if (isSelected) {
		flags |= ImGuiTreeNodeFlags_Selected; // 選択時は選択状態にする
	}
	if (isOpen) {
		flags |= ImGuiTreeNodeFlags_DefaultOpen;
	}
	if (children.empty()) {
		flags |= ImGuiTreeNodeFlags_Leaf;
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

nlohmann::json RemoteWorldInstance::serialize() const {
	nlohmann::json result;

	result["Type"] = 0;
	result["Name"] = hierarchyName;
	result["Transform"] = transform;
	result["Children"] = nlohmann::json::array();
	for (const auto& child : children) {
		result["Children"].emplace_back(child->serialize());
	}

	return result;
}

#endif // DEBUG_FEATURES_ENABLE
