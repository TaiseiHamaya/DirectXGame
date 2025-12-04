#pragma once

#include <filesystem>
#include <vector>

#include <Library/Externals/DirectXTex/DirectXTex.h>
#include <Library/Externals/msdf-atlas-gen/msdf-atlas-gen/msdf-atlas-gen.h>

class FontAtlasBuilder final {
public:
	void entry_point(const std::filesystem::path& path);

private:
	/// <summary>
	/// フォントデータの読み込み
	/// </summary>
	void load_glyphs();

	/// <summary>
	/// mtsdfテクスチャをdds形式で保存
	/// </summary>
	void save_mtsdf_texture();

	/// <summary>
	/// mtsdfテクスチャのatlasデータの保存
	/// </summary>
	void save_atlas_data();

private:
	std::filesystem::path ttfFilePath;

	msdf_atlas::ImmediateAtlasGenerator<float, 4, msdf_atlas::mtsdfGenerator, msdf_atlas::BitmapAtlasStorage<float, 4>> generator;
	std::vector<msdf_atlas::GlyphGeometry> glyphs;
	r32 baseFontScale;
	r32 lineHeight;

	DirectX::ScratchImage scratchImage;
};
