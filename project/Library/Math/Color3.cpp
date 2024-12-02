#include "Color3.h"


#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

#ifdef _DEBUG
void Color3::debug_gui() noexcept(false) {
	ImGui::ColorEdit3("Color3", &(this->red), ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB);
}
#endif // _DEBUG
