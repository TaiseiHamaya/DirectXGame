#include "Color.h"

//#include "Easing.h"

#include <algorithm>

#include <externals/imgui/imgui.h>

Color::Color() {
	red = 0;
	green = 0;
	blue = 0;
	alpha = 0;
}

Color::Color(unsigned int hex) {
	red = ((hex >> 24) & 0x000000ff) / 255.0f;
	green = ((hex >> 16) & 0x000000ff) / 255.0f;
	blue = ((hex >> 8) & 0x000000ff) / 255.0f;
	alpha = ((hex >> 0) & 0x000000ff) / 255.0f;
}

Color::Color(unsigned int _red, unsigned int _green, unsigned int _blue, float _alpha) {
	red = _red / 255.0f;
	green = _green / 255.0f;
	blue = _blue / 255.0f;
	alpha = _alpha / 255.0f;
}

Color::Color(float _red, float _green, float _blue, float _alpha) {
	red = _red;
	green = _green;
	blue = _blue;
	alpha = _alpha;
}

const unsigned int Color::hex() const {
	return (uint32_t(red * 255) << 24) + (uint32_t(green * 255) << 16) + (uint32_t(blue * 255) << 8) + uint32_t(alpha * 255);
}

#ifdef _DEBUG
void Color::debug_gui() {
	ImGui::ColorEdit4("Color", &(this->red), ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_InputRGB);
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
