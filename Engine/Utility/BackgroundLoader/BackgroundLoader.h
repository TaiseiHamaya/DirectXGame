#pragma once


#include <d3d12.h>
#include <wrl/client.h>

#include <list>
#include <memory>
#include <string>
#include <thread>
#include <variant>

class Texture;
class PolygonMesh;

enum class LoadEvent {
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
	static void WaitEndExecute();
	static bool IsLoading();

private:
	void initialize();
	void load_manager();
	void create_texture_view();
	void transfer_data();

private:
	// ロード用
	// ロード待機中のデータ
	struct LoadingQue {
		std::string filePath;
		std::string fileName;

		// variant用定義
		struct LoadTextureData {
			std::shared_ptr<Texture> textureData;
			Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource;
		};
		struct LoadPolygonMeshData {
			std::shared_ptr<PolygonMesh> meshData;
		};

		// ロードデータ
		std::variant<LoadTextureData, LoadPolygonMeshData, void*> loadData;
	};

	// イベント1つ分
	struct EventList {
		LoadEvent eventId;
		std::unique_ptr<LoadingQue> data;
	};
	// ロード用スレッド
	std::thread loadFunc;
	// イベント一覧
	std::list<EventList> loadEvents;
	// ロードが終わって転送待ち
	std::list<EventList> waitLoadingQue;
	// マルチスレッド終了判定用
	bool isEndProgram;
	bool isLoading;
};

