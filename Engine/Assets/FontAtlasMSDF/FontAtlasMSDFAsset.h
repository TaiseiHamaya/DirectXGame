#pragma once

#include <string_view>
#include <unordered_map>
#include <vector>

#include <Library/Math/Matrix3x3.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/StructuredBuffer/StructuredBuffer.h"

class TextureAsset;
struct GlyphRenderingData;

class FontAtlasMSDFAsset final {
public:
	struct GlyphDataGpu {
		Matrix3x3 uvMatrix;
		struct {
			r32 top;
			r32 left;
			r32 bottom;
			r32 right;
		} bounds;
	};

	struct GlyphData {
		r32 advance;
	};

public:
	FontAtlasMSDFAsset(std::vector<std::pair<GlyphData, GlyphDataGpu>>&& glyphsData_, std::unordered_map<u32, u32>&& glyphMap_, std::shared_ptr<const TextureAsset> texture_);
	~FontAtlasMSDFAsset();

	__CLASS_NON_COPYABLE(FontAtlasMSDFAsset)

public:
	const std::optional<u32>& glyph_bindless_index() const;
	std::optional<u32> texture_bindless_index() const;

public:
	std::vector<GlyphRenderingData> calculate_glyph(std::string_view string) const;

private:
	std::vector<GlyphData> glyphsData;
	StructuredBuffer<GlyphDataGpu> glyphBuffers;
	std::shared_ptr<const TextureAsset> textureAsset;
	std::unordered_map<u32, u32> glyphMap;
};
