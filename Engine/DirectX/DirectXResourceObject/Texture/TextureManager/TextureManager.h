#pragma once

#include <d3d12.h>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <wrl/client.h>

class Texture;

class TextureManager final {
private:
	TextureManager();

public:
	~TextureManager();

private:
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

public:
	static TextureManager& GetInstance();

public:
	static void Initialize();
	static void RegisterLoadQue(const std::string& filePath, const std::string& textureName);

	static std::weak_ptr<Texture> GetTexture(const std::string& textureName);

	static void Transfer(const std::string& name, std::shared_ptr<Texture>& data);

private: // メンバ変数
	std::unordered_map<std::string, std::shared_ptr<Texture>> textureInstanceList;
	std::unordered_set<std::string> textureRegisteredList;
};

