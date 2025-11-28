#pragma once

#include <vector>
#include <string_view>
#include <unordered_map>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/StructuredBuffer/StructuredBuffer.h"

class TextureAsset;

class FontAtlasMSDFAsset final {
public:
	struct GlyphDataGpu {
		u32 top;
		u32 left;
		u32 bottom;
		u32 right;
	};

public:
	FontAtlasMSDFAsset(std::vector<GlyphDataGpu>&& glyphsData_, std::unordered_map<u32, u32>&& glyphMap_, std::shared_ptr<const TextureAsset> texture_);
	~FontAtlasMSDFAsset();

	__CLASS_NON_COPYABLE(FontAtlasMSDFAsset)

public:
	const std::optional<u32>& glyph_bindless_index() const;
	std::optional<u32> texture_bindless_index() const;
	std::vector<u32> calculate_glyph_indices(std::string_view string) const;

private:
	StructuredBuffer<GlyphDataGpu> glyphsData;
	std::shared_ptr<const TextureAsset> textureAsset;
	std::unordered_map<u32, u32> glyphMap;
};
