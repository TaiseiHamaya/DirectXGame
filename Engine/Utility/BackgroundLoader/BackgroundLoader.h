#pragma once


#include <d3d12.h>
#include <wrl/client.h>

#include <memory>
#include <string>
#include <thread>
#include <list>
#include <variant>

class Texture;
class PolygonMesh;

enum class LoadEvent {
	Execute,
	LoadTexture,
	LoadPolygonMesh,
};

class BackgroundLoader {
private:
	BackgroundLoader();

public:
	~BackgroundLoader();

private:
	BackgroundLoader(const BackgroundLoader&) = delete;
	BackgroundLoader& operator=(const BackgroundLoader&) = delete;

public:
	static BackgroundLoader& GetInstance();

public:
	static void Initialize();
	static void RegisterLoadQue(LoadEvent eventID, const std::string& filePath, const std::string& textureName);
	static void LoadImperative();
	static void WaitEndExecute();
	static void LoadImperativeAndWait();
	static bool IsLoading();

private:
	void initialize();
	void load_manager();
	void create_texture_view();
	void transfer_data();

private:
	// ロード用
	struct LoadingQue {
		std::string filePath;
		std::string fileName;
		struct LoadTextureData {
			std::shared_ptr<Texture> textureData;
			Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource;
		};
		struct LoadPolygonMeshData {
			std::shared_ptr<PolygonMesh> meshData;
		};
		std::variant<LoadTextureData, LoadPolygonMeshData, void*> loadData;
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

