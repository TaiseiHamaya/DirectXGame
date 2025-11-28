#ifdef DEBUG_FEATURES_ENABLE

#include "FontAtlasBuilder.h"

#include <thread>

#include "./FontAtlasBuilderManager.h"

#include "Engine/Assets/Json/JsonAsset.h"

using namespace msdf_atlas;

void FontAtlasBuilder::entry_point(const std::filesystem::path& path) {
	ttfFilePath = path;

	// ロード
	load_glyphs();

	save_mtsdf_texture();
	save_atlas_data();
}

void FontAtlasBuilder::load_glyphs() {
	msdfgen::FontHandle* font = loadFont(FontAtlasBuilderManager::FreetypeHandle(), ttfFilePath.string().c_str());
	if (!font) {
		return;
	}
	FontGeometry fontGeometry(&glyphs);
	fontGeometry.loadCharset(font, 1.0, FontAtlasBuilderManager::Charset());
	for (GlyphGeometry& glyph : glyphs) {
		glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, 3.0, 0);
	}


	TightAtlasPacker packer;
	constexpr int atlasSize = 1024;
	packer.setDimensions(atlasSize, atlasSize);
	packer.setMinimumScale(16.0);
	packer.setPixelRange(2.0);
	packer.setUnitRange(1.0);
	packer.pack(glyphs.data(), static_cast<int>(glyphs.size()));

	// 実際の生成
	generator.resize(atlasSize, atlasSize);
	GeneratorAttributes attributes;
	generator.setAttributes(attributes);
	generator.setThreadCount(std::thread::hardware_concurrency());
	generator.generate(glyphs.data(), static_cast<int>(glyphs.size()));

	// 削除
	destroyFont(font);
}

void FontAtlasBuilder::save_mtsdf_texture() {
	msdfgen::BitmapConstRef<byte, 4> bitmapRef = generator.atlasStorage();
	HRESULT hr;
	DirectX::Image image = {}; // 書き込み
	image.width = bitmapRef.width;
	image.height = bitmapRef.height;
	image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	image.rowPitch = sizeof(byte) * 4 * bitmapRef.width;
	image.slicePitch = image.rowPitch * bitmapRef.height;
	image.pixels = const_cast<uint8_t*>(bitmapRef.pixels);

	// ScratchImage に変換
	hr = scratchImage.InitializeFromImage(image);
	assert(SUCCEEDED(hr));

	// 圧縮
	DirectX::ScratchImage compressed;
	hr = DirectX::Compress(scratchImage.GetImages(), scratchImage.GetImageCount(), scratchImage.GetMetadata(),
		DXGI_FORMAT_BC7_UNORM, DirectX::TEX_COMPRESS_BC7_QUICK | DirectX::TEX_COMPRESS_PARALLEL, 1.0f, compressed);
	assert(SUCCEEDED(hr));

	// 転送
	scratchImage = std::move(compressed);
	DirectX::TexMetadata metadata = scratchImage.GetMetadata();

	// 保存
	std::filesystem::path outputPath = ttfFilePath.parent_path() / (ttfFilePath.stem().native() + L".dds");
	hr = DirectX::SaveToDDSFile(scratchImage.GetImages(), scratchImage.GetImageCount(), metadata, DirectX::DDS_FLAGS_NONE, outputPath.c_str());
	assert(SUCCEEDED(hr));
}

void FontAtlasBuilder::save_atlas_data() {
	JsonAsset json;
	std::filesystem::path outputPath = ttfFilePath.parent_path() / (ttfFilePath.stem().native() + L".mtsdf");
	json.load(outputPath);
	json.get().clear();

	// Glyphの情報を保存
	nlohmann::json atlasJson = nlohmann::json::array();
	for (const GlyphGeometry& glyph : glyphs) {
		nlohmann::json glyphJson;
		glyphJson["Codepoint"] = glyph.getCodepoint();
		int l, b, w, h;
		glyph.getBoxRect(l, b, w, h);
		glyphJson["Box"] = { {"Left", l}, {"Bottom", b}, {"Right", l + w}, {"Top", b + h} };
		glyphJson["Index"] = glyph.getIndex();
		atlasJson.push_back(glyphJson);
	}

	json.get()["Glyphs"] = atlasJson;

	json.get()["TextureWidth"] = scratchImage.GetMetadata().width;
	json.get()["TextureHeight"] = scratchImage.GetMetadata().height;

	json.get()["DDSTexture"] = ttfFilePath.stem().native() + L".dds";

	json.save();
}

#endif // DEBUG_FEATURES_ENABLE
