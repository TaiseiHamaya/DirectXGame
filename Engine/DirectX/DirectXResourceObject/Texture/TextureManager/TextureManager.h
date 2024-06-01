#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

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
	static bool IsRegistered(const std::string& textureName);

	static void Transfer(const std::string& name, std::shared_ptr<Texture>& data);

#ifdef _DEBUG
	static bool TextureListGui(std::string& current);
#endif // _DEBUG


private: // メンバ変数
	std::unordered_map<std::string, std::shared_ptr<Texture>> textureInstanceList;
	std::unordered_set<std::string> textureRegisteredList;
};

