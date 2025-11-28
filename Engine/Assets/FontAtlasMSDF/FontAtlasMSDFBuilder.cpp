#include "FontAtlasMSDFBuilder.h"

#include "../Texture/TextureAssetBuilder.h"
#include "../Texture/TextureLibrary.h"
#include "./FontAtlasMSDFLibrary.h"
#include "Engine/Assets/Json/JsonAsset.h"

FontAtlasMSDFBuilder::FontAtlasMSDFBuilder(const std::filesystem::path& filePath_) {
	filePath = IAssetBuilder::ResolveFilePath(filePath_, "FontAtlasMSDF");
}

FontAtlasMSDFBuilder::~FontAtlasMSDFBuilder() = default;

bool FontAtlasMSDFBuilder::run() {
	JsonAsset json;
	json.load(filePath);
	nlohmann::json glyphsJson = json.get()["Glyphs"];

	if (glyphsJson.empty()) {
		return false;
	}

	glyphsDataBuffer.resize(glyphsJson.size());

	for (i32 i = 0;  auto& glyphJson : glyphsJson) {
		FontAtlasMSDFAsset::GlyphDataGpu glyphData;
		const nlohmann::json& boxJson = glyphJson["Box"];
		glyphData.top = boxJson.value("Top", 0);
		glyphData.left = boxJson.value("Left", 0);
		glyphData.bottom = boxJson.value("Bottom", 0);
		glyphData.right = boxJson.value("Right", 0);
		u32 codepoint = glyphJson["Codepoint"].get<u32>();
		if (glyphMap.contains(codepoint)) {
			continue;
		}
		glyphsDataBuffer[i] = glyphData;
		glyphMap[codepoint] = i;
		++i;
	}

	ddsTextureName = json.get().value("DDSTexture", filePath.stem().string() + ".dds");

	textureBuilder = std::make_unique<TextureAssetBuilder>(filePath.parent_path() / ddsTextureName);
	if (!textureBuilder->run()) {
		return false;
	}
	return true;
}

void FontAtlasMSDFBuilder::postprocess() {
	textureBuilder->postprocess();
	auto texture = textureBuilder->texture_data();
	fontAtlas = std::make_shared<FontAtlasMSDFAsset>(
		std::move(glyphsDataBuffer),
		std::move(glyphMap),
		texture
	);
}

void FontAtlasMSDFBuilder::transfer() {
	// MTSDFテクスチャ転送
	textureBuilder->transfer();
	// MSDF Atlasの転送
	FontAtlasMSDFLibrary::Transfer(filePath.filename().string(), fontAtlas);
}
