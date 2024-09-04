#include "TextureManager.h"

#include <mutex>
#include <format>

#include "Engine/Utility/BackgroundLoader/BackgroundLoader.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/Texture.h"
#include "Engine/Utility/Utility.h"

#ifdef _DEBUG
#include "externals/imgui/imgui.h"
#endif // _DEBUG

std::mutex textureMutex;

TextureManager::TextureManager() = default;

TextureManager::~TextureManager() noexcept = default;

TextureManager& TextureManager::GetInstance() noexcept {
	static std::unique_ptr<TextureManager> instance{ new TextureManager };
	return *instance;
}

void TextureManager::Initialize() noexcept {
	GetInstance();
}

void TextureManager::Finalize() {
	auto&& end = GetInstance().textureInstanceList.end();
	for (auto&& itr = GetInstance().textureInstanceList.begin(); itr != end; ++itr) {
		itr->second->release_srv_heap();
	}
}

void TextureManager::RegisterLoadQue(const std::string& filePath, const std::string& textureName) {
	if (IsRegistered(textureName)) {
		return;
	}
	BackgroundLoader::RegisterLoadQue(LoadEvent::LoadTexture, filePath, textureName);
}

std::weak_ptr<Texture> TextureManager::GetTexture(const std::string& textureName) noexcept(false) {
	std::lock_guard<std::mutex> lock{ textureMutex };
	// 見つかったらそのデータのweak_ptrを返す
	if (IsRegisteredUnlocking(textureName)) {
		return GetInstance().textureInstanceList.at(textureName);
	}
	else {
		Log(std::format("[TextureManager] Texture Name-\'{:}\' is not loading.\n", textureName));
		return GetInstance().textureInstanceList.at("Error.png");
	}
}

bool TextureManager::IsRegistered(const std::string& textureName) noexcept(false) {
	std::lock_guard<std::mutex> lock{ textureMutex };
	return IsRegisteredUnlocking(textureName);
}

void TextureManager::UnloadTexture(const std::string& textureName) {
	std::lock_guard<std::mutex> lock{ textureMutex };
	if (IsRegisteredUnlocking(textureName)) {
		Log(std::format("[TextureManager] Unload texture Name-\'{:}\'.\n", textureName));
		auto&& texture = GetInstance().textureInstanceList.at(textureName);
		texture->release_srv_heap();
		texture.reset();
		GetInstance().textureInstanceList.erase(textureName);
	}
}

void TextureManager::Transfer(const std::string& name, std::shared_ptr<Texture>& data) {
	std::lock_guard<std::mutex> lock{ textureMutex };
	if (IsRegisteredUnlocking(name)) {
		data->release_srv_heap();
		Log(std::format("[TextureManager] Transferring registered texture. Name-\'{:}\', Address-\'{:}\'\n", name, (void*)data.get()));
		return;
	}
	Log(std::format("[TextureManager] Transfer new Texture. Name-\'{:}\', Address-\'{:}\'\n", name, (void*)data.get()));
	GetInstance().textureInstanceList.emplace(name, data);
}

#ifdef _DEBUG
bool TextureManager::TextureListGui(std::string& current) {
	bool changed = false;

	std::lock_guard<std::mutex> lock{ textureMutex };
	if (ImGui::BeginCombo("TextureList", current.c_str())) {
		auto&& list = GetInstance().textureInstanceList;
		for (auto itr = list.begin(); itr != list.end(); ++itr) {
			bool is_selected = (current == itr->first);
			if (ImGui::Selectable(itr->first.c_str(), is_selected)) {
				current = itr->first;
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

bool TextureManager::IsRegisteredUnlocking(const std::string& textureName) noexcept(false) {
	return GetInstance().textureInstanceList.contains(textureName);
}
