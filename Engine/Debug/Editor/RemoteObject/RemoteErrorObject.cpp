#include "RemoteErrorObject.h"

#include <format>

#include <imgui.h>

#include "Engine/Application/Output.h"

#include "../Command/EditorCommandInvoker.h"
#include "../Command/EditorSelectCommand.h"

void RemoteErrorObject::draw_inspector() {
	ImGui::Text("Missing RemoteObject");
	ImGui::NewLine();
	ImGui::Separator();
}

void RemoteErrorObject::draw_hierarchy(Reference<const EditorSelectObject> select) {
	bool isSelected = select->is_selected(this);

	if (ImGui::Selectable(std::format("Missing RemoteObject.##{}", (void*)this).c_str(), isSelected)) {
		EditorCommandInvoker::Execute(
			std::make_unique<EditorSelectCommand>(this)
		);
	}
}

std::unique_ptr<IRemoteObject> RemoteErrorObject::move_force([[maybe_unused]] Reference<const IRemoteObject> child) {
	return nullptr;
}

void RemoteErrorObject::reparent(Reference<IRemoteObject> remoteObject) {
	parent = remoteObject;
}

void RemoteErrorObject::add_child(std::unique_ptr<IRemoteObject> child) {
	Warning("RemoteErrorObject cannot have children.");
}

nlohmann::json RemoteErrorObject::serialize() const {
	nlohmann::json result;

	result["Type"] = 99;
	result["Name"] = hierarchyName;

	return result;
}
