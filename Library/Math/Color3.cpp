#include "Color3.h"


#ifdef DEBUG_FEATURES_ENABLE
#include <imgui.h>
#endif // _DEBUG

#ifdef DEBUG_FEATURES_ENABLE
void Color3::debug_gui(string_literal tag) noexcept(false) {
	ImGui::ColorEdit3(tag, &(this->red), ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB);
}
#endif // _DEBUG
