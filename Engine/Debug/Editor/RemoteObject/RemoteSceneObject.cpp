#ifdef DEBUG_FEATURES_ENABLE

#include "RemoteSceneObject.h"

#include <format>

#include "Engine/Runtime/Scene/BaseScene.h"
#include "RemoteWorldRootObject.h"

#include <imgui.h>
#include <imgui_stdlib.h>

RemoteSceneObject::RemoteSceneObject() = default;
RemoteSceneObject::~RemoteSceneObject() = default;

void RemoteSceneObject::initialize(Reference<BaseScene> self_) {
	self = self_;
	worldRoots.resize(self->world_size());
	for (i32 i = 0; i < worldRoots.size(); ++i) {
		worldRoots[i].initialize(self->get_world(i));
	}
}

void RemoteSceneObject::draw_inspector() {
	ImGui::InputText("SceneName", &hierarchyName);
}

void RemoteSceneObject::draw_hierarchy(Reference<IRemoteObject>& select) {
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
		for (auto& world: worldRoots) {
			world.draw_hierarchy(select);
		}
		ImGui::TreePop();
	}
}

#endif // DEBUG_FEATURES_ENABLE
