#pragma once

#include <string>
#include <thread>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <d3d12.h>
#include <wrl.h>

class Texture;
enum class LoadStatus {
	InQueue,
	SetCommand,
	WaitExecute,
	EndLoad
};

enum class LoadEvent {
	Execute,
	EmplaceData,
};

class TextureManager {
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
	static void LoadImperative();
	static void WaitEndExecute();
	static void LoadImperativeAndWait();

	static std::weak_ptr<Texture> GetTexture(const std::string& textureName);

private:
	void initialize();
	void load_manager();
	void create_view();

private: // メンバ変数
	std::unordered_map<std::string, std::shared_ptr<Texture>> textureInstanceList;
	std::unordered_set<std::string> textureRegisteredList;
	
	// ロード用
	struct LoadingQue {
		std::string filePath;
		std::string fileName;
		LoadStatus status;
		std::shared_ptr<Texture> texture;
		Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource;
	};
	struct EventList {
		LoadEvent eventId;
		std::unique_ptr<LoadingQue> que;
	};
	std::thread loadFunc;

	std::list<EventList> loadEvents;
	std::list<EventList> waitLoadingQue;
	bool isEndProgram;
	bool isExecuting;
};

