#include "RemoteErrorObject.h"

#include <imgui.h>

void RemoteErrorObject::draw_inspector() {
	ImGui::Text("Missing RemoteObject");
	ImGui::NewLine();
	ImGui::Separator();
}

void RemoteErrorObject::draw_hierarchy(Reference<IRemoteObject>& select) {
	bool isSelected = this == select.ptr();

	if (ImGui::Selectable(std::format("Missing RemoteObject.##{}", (void*)this).c_str(), isSelected)) {
		select = this;
	}
}
