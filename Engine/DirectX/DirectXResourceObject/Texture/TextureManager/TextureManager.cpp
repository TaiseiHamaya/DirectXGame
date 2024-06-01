#include "TextureManager.h"

#include <cassert>
#include <mutex>

#include "Engine/Utility/BackgroundLoader/BackgroundLoader.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/Texture.h"

#ifdef _DEBUG
#include "externals/imgui/imgui.h"
#endif // _DEBUG

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
	// 見つかったらそのデータのweak_ptrを返す
	if (IsRegistered(textureName))
		return GetInstance().textureInstanceList.at(textureName);
	else
		return GetInstance().textureInstanceList.at("Error.png");
}

bool TextureManager::IsRegistered(const std::string& textureName) {
	return GetInstance().textureRegisteredList.find(textureName) != GetInstance().textureRegisteredList.end();
}

void TextureManager::Transfer(const std::string& name, std::shared_ptr<Texture>& data) {
	std::lock_guard<std::mutex> lock(textureMutex);
	GetInstance().textureInstanceList.emplace(name, data);
	GetInstance().textureRegisteredList.emplace(name);
}

#ifdef _DEBUG
bool TextureManager::TextureListGui(std::string& current) {
	bool changed = false;

	std::lock_guard<std::mutex> lock(textureMutex);
	if (ImGui::BeginCombo("TextureList", current.c_str())) {
		auto&& list = GetInstance().textureRegisteredList;
		for (auto itr = list.begin(); itr != list.end(); ++itr) {
			bool is_selected = (current == *itr);
			if (ImGui::Selectable(itr->c_str(), is_selected)) {
				current = *itr;
				changed = true;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();

	}
	return changed;
}
#endif // _DEBUG
