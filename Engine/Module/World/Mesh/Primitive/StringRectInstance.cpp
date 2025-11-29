#include "StringRectInstance.h"

#include <Engine/Assets/FontAtlasMSDF/FontAtlasMSDFLibrary.h>

StringRectInstance::StringRectInstance() noexcept = default;

StringRectInstance::~StringRectInstance() noexcept = default;

void StringRectInstance::initialize(const std::string& msdfFont, r32 fontSize_, const Vector2& pivot_) {
	data.fondSize = fontSize_;
	data.pivot = pivot_;
	fontAtlas = FontAtlasMSDFLibrary::Get(msdfFont);
}

void StringRectInstance::set_font_size(r32 fontSize) {
	data.fondSize = fontSize;
}

r32 StringRectInstance::font_size() const {
	return data.fondSize;
}

void StringRectInstance::set_pivot(const Vector2& pivot) {
	data.pivot = pivot;
}

const Vector2& StringRectInstance::pivot_imm() const {
	return data.pivot;
}

void StringRectInstance::set_string(std::string_view string_) {
	string = string_;
	charRenderingData.clear();
	charRenderingData = fontAtlas->calculate_glyph(string);
}

const std::string& StringRectInstance::string_imm() const {
	return string;
}

const std::vector<GlyphRenderingData>& StringRectInstance::glyph_data_imm() const {
	return charRenderingData;
}

u32 StringRectInstance::font_texture_index() const {
	if (!fontAtlas) {
		return 0;
	}
	auto textureIndexOpt = fontAtlas->texture_bindless_index();
	if (!textureIndexOpt.has_value()) {
		return 0;
	}
	return textureIndexOpt.value();
}

std::optional<u32> StringRectInstance::glyph_bindless_index() const {
	if (!fontAtlas) {
		return std::nullopt;
	}
	return fontAtlas->glyph_bindless_index();
}
