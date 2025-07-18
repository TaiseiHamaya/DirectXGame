#ifdef DEBUG_FEATURES_ENABLE

#include "RemoteSceneObject.h"

#include <format>

#include <imgui.h>

#include "Engine/Application/Output.h"

#include "../Command/EditorCommandInvoker.h"
#include "../Command/EditorSelectCommand.h"
#include "IRemoteObject.h"
#include "RemoteWorldObject.h"
#include "../Window/EditorSceneView.h"

RemoteSceneObject::RemoteSceneObject() = default;
RemoteSceneObject::~RemoteSceneObject() = default;

void RemoteSceneObject::setup() {
	for (auto& child : remoteWorlds) {
		child->setup();
	}
}

void RemoteSceneObject::update_preview(Reference<RemoteWorldObject> world, Reference<Affine> parentAffine) {
	CriticalIf(world || parentAffine, "RemoteSceneObject::update_preview's argument named \'world\' and \'parentAffine\' must be nullptr.");
	for (auto& child : remoteWorlds) {
		child->update_preview(nullptr, nullptr);
	}
}

void RemoteSceneObject::draw_inspector() {
	hierarchyName.show_gui();

	ImGui::Separator();

	numLayer.show_gui();
}

void RemoteSceneObject::draw_hierarchy(Reference<const EditorSelectObject> select) {
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

	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen() && !isSelected) {
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

std::unique_ptr<IRemoteObject> RemoteSceneObject::move_force(Reference<const IRemoteObject> child) {
	auto itr = std::find_if(remoteWorlds.begin(), remoteWorlds.end(),
	[&](const std::unique_ptr<RemoteWorldObject>& lhs) {
		return lhs.get() == child.ptr();
	});
	if (itr != remoteWorlds.end()) {
		std::unique_ptr<IRemoteObject> world = std::move(*itr);
		remoteWorlds.erase(itr);
		return world;
	}
	return nullptr;
}

void RemoteSceneObject::reparent(Reference<IRemoteObject> remoteObject) {
	Error("RemoteSceneObject is must be root object.");
}

void RemoteSceneObject::add_child(std::unique_ptr<IRemoteObject> child) {
	auto tmp = dynamic_cast<RemoteWorldObject*>(child.release());
	auto childPtr = std::unique_ptr<RemoteWorldObject>(tmp);
	if (!childPtr) {
		Warning("RemoteSceneObject can only add RemoteWorldObject as child.");
		return;
	}
	childPtr->reparent(this);
	remoteWorlds.emplace_back(std::move(childPtr));
}

nlohmann::json RemoteSceneObject::serialize() const {
	nlohmann::json result;

	result.update(hierarchyName);
	result["Worlds"] = nlohmann::json::array();
	for (const auto& world : remoteWorlds) {
		result["Worlds"].emplace_back(world->serialize());
	}

	result.update(numLayer);

	return result;
}

Reference<const RemoteWorldObject> RemoteSceneObject::query_world() const {
	Warning("IRemoteObject::query_world() was called in RemoteSceneObject.");
	return nullptr;
}

void RemoteSceneObject::on_spawn() {
	for (auto& child : remoteWorlds) {
		if (child) {
			child->on_spawn();
		}
	}
}

void RemoteSceneObject::on_destroy() {
	for (auto& child : remoteWorlds) {
		if (child) {
			child->on_destroy();
		}
	}
}

size_t RemoteSceneObject::world_size() const {
	return remoteWorlds.size();
}

std::string RemoteSceneObject::name() const {
	return hierarchyName.copy();
}

const std::vector<std::unique_ptr<RemoteWorldObject>>& RemoteSceneObject::get_remote_worlds() const {
	return remoteWorlds;
}

#endif // DEBUG_FEATURES_ENABLE
