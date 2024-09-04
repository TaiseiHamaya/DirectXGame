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
class AudioResource;

enum class LoadEvent {
	LoadTexture,
	LoadPolygonMesh,
	LoadAudio,
};

/// <summary>
/// バックグラウンドロード用クラス
/// </summary>
class BackgroundLoader final {
private:
	BackgroundLoader() noexcept;

public:
	~BackgroundLoader() noexcept;

private: // コピームーブ禁止
	BackgroundLoader(const BackgroundLoader&) = delete;
	BackgroundLoader& operator=(const BackgroundLoader&) = delete;

public: // シングルトン
	static BackgroundLoader& GetInstance() noexcept;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// ロードイベントの登録
	/// </summary>
	/// <param name="eventID">LoadEventID</param>
	/// <param name="filePath">ファイルパス</param>
	/// <param name="fileName">ファイル名</param>
	static void RegisterLoadQue(LoadEvent eventID, const std::string& filePath, const std::string& fileName) noexcept(false);

	/// <summary>
	/// ロードが完了するまで待機
	/// </summary>
	static void WaitEndExecute();

	/// <summary>
	/// ロード中か取得
	/// </summary>
	/// <returns></returns>
	static bool IsLoading() noexcept;

private:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() noexcept(false);

	/// <summary>
	/// ロード管理用関数
	/// </summary>
	void load_manager();

	/// <summary>
	/// 読み込み済みテクスチャのビューを作成
	/// </summary>
	void create_texture_view();

	/// <summary>
	/// ロード済みデータを各Managerに転送
	/// </summary>
	void transfer_data();

private:
	/// <summary>
	/// ロード用キュー
	/// </summary>
	struct LoadingQue {
		std::string filePath; // パス
		std::string fileName; // ファイル名

		// variant用定義
		struct LoadTextureData {
			std::shared_ptr<Texture> textureData; // 実データ
			Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource; // 一時リソース
		};
		struct LoadPolygonMeshData {
			std::shared_ptr<PolygonMesh> meshData; // メッシュデータ
		};
		struct LoadAudioData {
			std::shared_ptr<AudioResource> audioData; // メッシュデータ
		};
		std::variant<LoadTextureData, LoadPolygonMeshData, LoadAudioData, void*> loadData; // variantでDataを選択
	};

	/// <summary>
	/// イベントデータ
	/// </summary>
	struct EventList {
		LoadEvent eventId; // イベントID
		std::unique_ptr<LoadingQue> data; // 実データ
	};

	/// <summary>
	/// ロードスレッド
	/// </summary>
	std::thread loadFunc;
	
	/// <summary>
	/// イベント一覧
	/// </summary>
	std::list<EventList> loadEvents;

	/// <summary>
	/// ロードが終わって転送待ち
	/// </summary>
	std::list<EventList> waitLoadingQue;
	
	/// <summary>
	/// マルチスレッド終了判定用
	/// </summary>
	bool isEndProgram;

	/// <summary>
	/// ロード中フラグ
	/// </summary>
	bool isLoading;
};
