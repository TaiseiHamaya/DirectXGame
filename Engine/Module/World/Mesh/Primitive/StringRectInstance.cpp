#include "StringRectInstance.h"

#include "Engine/Assets/FontAtlasMSDF/FontAtlasMSDFLibrary.h"

StringRectInstance::StringRectInstance() noexcept = default;

StringRectInstance::~StringRectInstance() noexcept = default;

void StringRectInstance::initialize(const std::string& msdfFont, r32 fontSize_, const Vector2& pivot_) {
	data.fontSize = fontSize_;
	data.pivot = pivot_;
	fontAtlas = FontAtlasMSDFLibrary::Get(msdfFont);
}

void StringRectInstance::set_font_size(r32 fontSize) {
	data.fontSize = fontSize;
}

r32 StringRectInstance::font_size() const {
	return data.fontSize;
}

r32 StringRectInstance::font_scale() const {
	return data.fontSize / fontAtlas->base_scale();
}

void StringRectInstance::set_font(const std::string& fontName) {
	fontAtlas = FontAtlasMSDFLibrary::Get(fontName);
	if (!fontAtlas) {
		return;
	}
	charRenderingData = fontAtlas->calculate_glyph(string, data.fontSize);
	data.offset = fontAtlas->calculate_offset(charRenderingData, data.pivot, data.fontSize);
}

void StringRectInstance::set_pivot(const Vector2& pivot) {
	data.pivot = pivot;
	if (!fontAtlas) {
		return;
	}
	data.offset = fontAtlas->calculate_offset(charRenderingData, data.pivot, data.fontSize);
}

const Vector2& StringRectInstance::pivot_imm() const {
	return data.pivot;
}

const Vector2& StringRectInstance::offset_imm() const {
	return data.offset;
}

void StringRectInstance::set_string(std::string_view string_) {
	string = string_;
	charRenderingData.clear();
	if (!fontAtlas) {
		return;
	}
	charRenderingData = fontAtlas->calculate_glyph(string, data.fontSize);
	data.offset = fontAtlas->calculate_offset(charRenderingData, data.pivot, data.fontSize);
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

#ifdef DEBUG_FEATURES_ENABLE

#include <imgui.h>
#include <imgui_stdlib.h>

void StringRectInstance::debug_gui() {
	transform.debug_gui();

	ImGui::Separator();

	material.color.debug_gui();

	ImGui::Separator();

	ImGui::DragFloat("Font Size", &data.fontSize, 0.1f, 0.0f, 100000.0f);
	if (ImGui::DragFloat2("Pivot", &data.pivot.x, 0.1f, -1000.0f, 1000.0f)) {
		data.offset = fontAtlas->calculate_offset(charRenderingData, data.pivot, data.fontSize);
	}
	if (ImGui::InputTextMultiline("String", &string)) {
		charRenderingData = fontAtlas->calculate_glyph(string, data.fontSize);
		data.offset = fontAtlas->calculate_offset(charRenderingData, data.pivot, data.fontSize);
	}
}

#endif // DEBUG_FEATURES_ENABLE
