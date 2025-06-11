#include "RemoteErrorObject.h"

#include <format>

#include <imgui.h>

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
