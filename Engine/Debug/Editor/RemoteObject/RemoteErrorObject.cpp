#include "RemoteErrorObject.h"

#include <format>

#include <imgui.h>

void RemoteErrorObject::draw_inspector() {
	ImGui::Text("Missing RemoteObject");
	ImGui::NewLine();
	ImGui::Separator();
}

void RemoteErrorObject::draw_hierarchy(Reference<EditorSelectObject> select) {
	bool isSelected = select->is_selected(this);

	if (ImGui::Selectable(std::format("Missing RemoteObject.##{}", (void*)this).c_str(), isSelected)) {
		//select = this;
	}
}
