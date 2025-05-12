#ifdef DEBUG_FEATURES_ENABLE

#include "IEditorWindow.h"

#include <string>

#include <imgui.h>

void IEditorWindow::draw_menu(string_literal name) {
	std::string itemName = name;
	if (isActive) {
		itemName += "\ue5ca";
	}
	if (ImGui::MenuItem(itemName.c_str())) {
		isActive = true;
	}
}

#endif // DEBUG_FEATURES_ENABLE
