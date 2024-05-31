#include "TextureManager.h"

#include <cassert>
#include <mutex>

#include "Engine/Utility/BackgroundLoader/BackgroundLoader.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/Texture.h"

std::mutex textureMutex;

TextureManager::TextureManager() = default;

TextureManager::~TextureManager() = default;

TextureManager& TextureManager::GetInstance() {
	static std::unique_ptr<TextureManager> instance{ new TextureManager };
	return *instance;
}

void TextureManager::Initialize() {
	GetInstance();
}

void TextureManager::RegisterLoadQue(const std::string& filePath, const std::string& textureName) {
	std::lock_guard<std::mutex> lock(textureMutex);
	if (IsRegistered(textureName)) {
		return;
	}
	BackgroundLoader::RegisterLoadQue(LoadEvent::LoadTexture, filePath, textureName);
}

std::weak_ptr<Texture> TextureManager::GetTexture(const std::string& textureName) {
	std::lock_guard<std::mutex> lock(textureMutex);
	// 存在しないならエラー
	assert(IsRegistered(textureName));
	// 見つかったらそのデータのweak_ptrを返す
	return GetInstance().textureInstanceList.at(textureName);
}

bool TextureManager::IsRegistered(const std::string& textureName) {
	return GetInstance().textureRegisteredList.find(textureName) != GetInstance().textureRegisteredList.end();
}

void TextureManager::Transfer(const std::string& name, std::shared_ptr<Texture>& data) {
	std::lock_guard<std::mutex> lock(textureMutex);
	GetInstance().textureInstanceList.emplace(name, data);
	GetInstance().textureRegisteredList.emplace(name);
}
