#include "Color.h"

//#include "Easing.h"

//#include <algorithm>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

#ifdef _DEBUG
void Color::debug_gui3() noexcept(false) {
	ImGui::ColorEdit3("Color", &(this->red), ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_InputRGB);
}
void Color::debug_gui4() noexcept(false) {
	ImGui::ColorEdit4("Color", &(this->red), ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_InputRGB);
}
#endif // _DEBUG

//Color Color::LerpC(const Color& color1, const Color& color2, float t) {
//	return Color{
//		Lerp(color1.red, color2.red, t),
//		Lerp(color1.green, color2.green, t),
//		Lerp(color1.blue, color2.blue, t),
//		Lerp(color1.alpha, color2.alpha, t)
//	};
//}
