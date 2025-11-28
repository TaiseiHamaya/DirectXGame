#include "FontAtlasMSDFAsset.h"

#include <Library/Externals/msdf-atlas-gen/msdf-atlas-gen/msdf-atlas-gen.h>

#include "Engine/Assets/Texture/TextureAsset.h"

FontAtlasMSDFAsset::FontAtlasMSDFAsset(std::vector<GlyphDataGpu>&& glyphsData_, std::unordered_map<u32, u32>&& glyphMap_, std::shared_ptr<const TextureAsset> texture_) :
	glyphMap(std::move(glyphMap_)),
	textureAsset(texture_) {
	glyphsData.initialize(static_cast<u32>(glyphsData_.size()));
	for (u32 i = 0; i < static_cast<u32>(glyphsData_.size()); ++i) {
		glyphsData[i] = glyphsData_[i];
	}
}

FontAtlasMSDFAsset::~FontAtlasMSDFAsset() = default;

const std::optional<u32>& FontAtlasMSDFAsset::glyph_bindless_index() const {
	return glyphsData.index();
}

std::optional<u32> FontAtlasMSDFAsset::texture_bindless_index() const {
	if (!textureAsset) {
		return std::nullopt;
	}
	return textureAsset->index();
}

std::vector<u32> FontAtlasMSDFAsset::calculate_glyph_indices(std::string_view string) const {
	std::vector<u32> codepoints;
	msdf_atlas::utf8Decode(codepoints, string.data());

	std::vector<u32> indices;
	for (u32& codepoint : codepoints) {
		if (glyphMap.contains(codepoint)) {
			indices.emplace_back(glyphMap.at(codepoint));
		}
		else {
			indices.emplace_back(0); // 未登録文字は0番にフォールバック
		}
	}
	return indices;
}
