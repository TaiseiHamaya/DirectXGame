#include "TextureAssetBuilder.h"

#include <Library/Utility/Tools/SmartPointer.h>

#include "./TextureLibrary.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/Texture/Texture.h"

TextureAssetBuilder::TextureAssetBuilder(const std::filesystem::path& filePath_) {
	filePath = filePath_;
}

void TextureAssetBuilder::preprocess() {
	// メモリ確保
	textureData = eps::CreateShared<Texture>();
}

bool TextureAssetBuilder::run() {
	// 通常読み込み
	intermediateResource = textureData->load(filePath);
	return intermediateResource;
}

void TextureAssetBuilder::postprocess() {
	// Viewの作成
	textureData->create_resource_view();
	// GPU上の名前を設定
	textureData->set_name(filePath.filename().string());
}

void TextureAssetBuilder::transfer() {
	TextureLibrary::Transfer(filePath.filename().string(), textureData);
}
