#ifdef DEBUG_FEATURES_ENABLE

#include "RemoteWorldRootObject.h"

#include <format>

#include "Engine/Module/World/WorldManager.h"
#include "FolderObject.h"
#include "RemoteWorldInstance.h"

#include <imgui.h>
#include <imgui_stdlib.h>

RemoteWorldRootObject::RemoteWorldRootObject() = default;
RemoteWorldRootObject::~RemoteWorldRootObject() = default;

RemoteWorldRootObject::RemoteWorldRootObject(const RemoteWorldRootObject&) = default;
RemoteWorldRootObject& RemoteWorldRootObject::operator=(const RemoteWorldRootObject&) = default;
RemoteWorldRootObject::RemoteWorldRootObject(RemoteWorldRootObject&&) noexcept = default;
RemoteWorldRootObject& RemoteWorldRootObject::operator=(RemoteWorldRootObject&&) noexcept = default;

void RemoteWorldRootObject::initialize(Reference<WorldManager> self_) {
	self = self_;
	for (auto& instance : self->get_world_instances()[0]) {
		auto& remoteObject = childrenInstances.emplace_back();
		remoteObject.initialize(instance);
	}
}

void RemoteWorldRootObject::draw_inspector() {
	ImGui::InputText("World", &hierarchyName);
}

void RemoteWorldRootObject::draw_hierarchy(Reference<IRemoteObject>& select) {
	bool isSelected = this == select.ptr();

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
		select = this;
	}

	if (isOpen) {
		for (auto& child : childrenFolders) {
			child.draw_hierarchy(select);
		}
		for (auto& child : childrenInstances) {
			child.draw_hierarchy(select);
		}
		ImGui::TreePop();
	}
}

#endif // DEBUG_FEATURES_ENABLE
