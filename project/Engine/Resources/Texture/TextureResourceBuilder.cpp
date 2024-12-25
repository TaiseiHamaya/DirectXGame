#include "TextureResourceBuilder.h"

#include "Engine/Rendering/DirectX/DirectXResourceObject/Texture/Texture.h"
#include "Engine/Utility/Tools/SmartPointer.h"
#include "TextureManager.h"

TextureResourceBuilder::TextureResourceBuilder(const std::filesystem::path& filePath_) {
	filePath = filePath_;
}

void TextureResourceBuilder::preprocess() {
	// メモリ確保
	textureData = eps::CreateShared<Texture>();
}

bool TextureResourceBuilder::run() {
	// 通常読み込み
	intermediateResource = textureData->load(filePath);
	return intermediateResource;
}

void TextureResourceBuilder::postprocess() {
	// Viewの作成
	textureData->create_resource_view();
	// GPU上の名前を設定
	textureData->set_name(filePath.filename().string());
}

void TextureResourceBuilder::transfer() {
	TextureManager::Transfer(filePath.filename().string(), textureData);
}
