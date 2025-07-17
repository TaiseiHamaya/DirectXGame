#ifdef DEBUG_FEATURES_ENABLE

#include "RemoteWorldObject.h"

#include <format>

#include <imgui.h>

#include "../Command/EditorCommandInvoker.h"
#include "../Command/EditorSelectCommand.h"
#include "../Core/EditorHierarchyDandD.h"
#include "../Window/EditorWorldView/EditorWorldView.h"

RemoteWorldObject::RemoteWorldObject() = default;
RemoteWorldObject::~RemoteWorldObject() = default;

void RemoteWorldObject::draw_inspector() {
	hierarchyName.show_gui();
}

void RemoteWorldObject::draw_hierarchy(Reference<const EditorSelectObject> select) {
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
	isOpen = ImGui::TreeNodeEx(std::format("{}##{}", hierarchyName.get(), (void*)this).c_str(), flags);
	if (ImGui::BeginDragDropTarget()) {
		EditorHierarchyDandD::EndDrag(this);
		ImGui::EndDragDropTarget();
	}

	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen() && !isSelected) {
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

std::unique_ptr<IRemoteObject> RemoteWorldObject::move_force(Reference<const IRemoteObject> child) {
	auto itr = std::find_if(children.begin(), children.end(),
		[&](const std::unique_ptr<IRemoteObject>& lhs) {
		return lhs.get() == child.ptr();
	});
	if (itr != children.end()) {
		std::unique_ptr<IRemoteObject> childObject = std::move(*itr);
		children.erase(itr);
		return childObject;
	}
	return nullptr;
}

void RemoteWorldObject::reparent(Reference<IRemoteObject> remoteObject) {
	parent = remoteObject;
}

void RemoteWorldObject::add_child(std::unique_ptr<IRemoteObject> child) {
	child->reparent(this);
	children.emplace_back(std::move(child));
}

nlohmann::json RemoteWorldObject::serialize() const {
	nlohmann::json result;

	result.update(hierarchyName);

	result["Instances"] = nlohmann::json::array();
	for (const auto& child : children) {
		if (child) {
			result["Instances"].emplace_back(child->serialize());
		}
	}

	return result;
}

void RemoteWorldObject::set_editor_world_view(Reference<EditorWorldView> worldView, Reference<const Affine>) {
	worldView->setup(this);
	if (!worldView->is_select_tab()) {
		return;
	}
	for (auto& child : children) {
		if (child) {
			child->set_editor_world_view(worldView);
		}
	}
}

const std::string& RemoteWorldObject::world_name() const {
	return hierarchyName.cget();
}

#endif // DEBUG_FEATURES_ENABLE
