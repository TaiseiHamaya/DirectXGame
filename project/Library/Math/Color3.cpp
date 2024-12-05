#include "Color3.h"


#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

#ifdef _DEBUG
void Color3::debug_gui(const char* tag) noexcept(false) {
	ImGui::ColorEdit3(tag, &(this->red), ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB);
}
#endif // _DEBUG
