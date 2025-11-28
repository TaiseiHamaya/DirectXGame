#pragma once

#include <filesystem>
#include <vector>

#include <Library/Externals/DirectXTex/DirectXTex.h>
#include <Library/Externals/msdf-atlas-gen/msdf-atlas-gen/msdf-atlas-gen.h>

class FontAtlasBuilder final {
public:
	void entry_point(const std::filesystem::path& path);

private:
	void load_glyphs();
	void save_mtsdf_texture();
	void save_atlas_data();

private:
	std::filesystem::path ttfFilePath;

	msdf_atlas::ImmediateAtlasGenerator<float, 4, msdf_atlas::mtsdfGenerator, msdf_atlas::BitmapAtlasStorage<byte, 4>> generator;
	std::vector<msdf_atlas::GlyphGeometry> glyphs;

	DirectX::ScratchImage scratchImage;
};
