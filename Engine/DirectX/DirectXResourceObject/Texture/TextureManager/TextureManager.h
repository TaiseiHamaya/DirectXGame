#pragma once

#include <string>
#include <d3d12.h>
#include <unordered_map>
#include <deque>
#include "../Texture.h"

class TextureManager {
public:
	
public:
	static void Initialize();
	static void Finalize();
	static void LoadTexture(const std::string& textureName, const std::string& FilePath);
	static void SetCommand(const std::string& textureName, ID3D12GraphicsCommandList* const commandList);

public:

private:
	static TextureManager* instance;

private: // 動的関数

private: // メンバ変数
	struct TextureData {
		uint32_t heapIndex;
		Texture* textureResource;
	};
	std::unordered_map<std::string, > heapIndex;
	std::deque<uint32_t> releasedHeapIndex;
};

