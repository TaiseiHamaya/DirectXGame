#include "Color.h"

//#include "Easing.h"

#include <algorithm>

#include <externals/imgui/imgui.h>

const unsigned int Color::hex() const {
	return (std::uint32_t(red * 255) << 24) + (std::uint32_t(green * 255) << 16) + (std::uint32_t(blue * 255) << 8) + std::uint32_t(alpha * 255);
}

#ifdef _DEBUG
void Color::debug_gui() noexcept(false) {
	ImGui::ColorEdit3("Color", &(this->red), ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_InputRGB);
}
#endif // _DEBUG

unsigned int Color::ToHex(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha) {
	return (red << 24) + (green << 16) + (blue << 8) + alpha;
}
//
//unsigned int Color::LerpC(unsigned int hex1, unsigned int hex2, float t) {
//	return Color::ToHex(
//		std::clamp(Lerp((hex1 >> 24) & 0x000000ff, (hex2 >> 24) & 0x000000ff, t), 0x0u, 0xffffffff),
//		std::clamp(Lerp((hex1 >> 16) & 0x000000ff, (hex2 >> 16) & 0x000000ff, t), 0x0u, 0xffffffff),
//		std::clamp(Lerp((hex1 >> 8) & 0x000000ff, (hex2 >> 8) & 0x000000ff, t), 0x0u, 0xffffffff),
//		std::clamp(Lerp(((hex1 >> 0) & 0x000000ff), (hex2 >> 0) & 0x000000ff, t), 0x0u, 0xffffffff));
//}
//
//Color Color::LerpC(const Color& color1, const Color& color2, float t) {
//	return Color{
//		Lerp(color1.red, color2.red, t),
//		Lerp(color1.green, color2.green, t),
//		Lerp(color1.blue, color2.blue, t),
//		Lerp(color1.alpha, color2.alpha, t)
//	};
//}
