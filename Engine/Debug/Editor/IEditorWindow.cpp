#ifdef DEBUG_FEATURES_ENABLE

#include "IEditorWindow.h"

#include <imgui.h>

void IEditorWindow::draw_menu(string_literal name) {
	if(ImGui::MenuItem(name)) {
		isActive = true;
	}
}

#endif // DEBUG_FEATURES_ENABLE
