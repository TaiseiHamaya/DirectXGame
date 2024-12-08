#include "Color4.h"

//#include "Easing.h"

#include <cmath>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

#ifdef _DEBUG
void Color4::debug_gui(const char* tag) noexcept(false) {
	ImGui::ColorEdit4(tag, &(this->red), ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_InputRGB);
}
#endif // _DEBUG

Color4 Color4::Lerp(const Color4& color1, const Color4& color2, float t) {
	return Color4{
		std::lerp(color1.red, color2.red, t),
		std::lerp(color1.green, color2.green, t),
		std::lerp(color1.blue, color2.blue, t),
		std::lerp(color1.alpha, color2.alpha, t)
	};
}
