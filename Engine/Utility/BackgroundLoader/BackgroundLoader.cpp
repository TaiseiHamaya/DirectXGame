#include "BackgroundLoader.h"

#include <cassert>
#include <functional>
#include <mutex>

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/Texture.h"
#include "Engine/Game/Audio/AudioResource.h"
#include "Engine/Game/Managers/AudioManager/AudioManager.h"
#include "Engine/Game/Managers/PolygonMeshManager/PolygonMeshManager.h"
#include "Engine/Game/Managers/TextureManager/TextureManager.h"
#include "Engine/Game/PolygonMesh/PolygonMesh.h"
#include "Engine/Utility/Utility.h"

std::mutex executeMutex;
std::mutex referenceMutex;
std::condition_variable waitConditionVariable;
std::condition_variable loadConditionVariable;

BackgroundLoader::BackgroundLoader() noexcept = default;

BackgroundLoader::~BackgroundLoader() noexcept {
	isEndProgram = true;
	if (loadFunc.joinable()) {
		loadConditionVariable.notify_all();
		loadFunc.join();
	}
}

BackgroundLoader& BackgroundLoader::GetInstance() noexcept {
	static std::unique_ptr<BackgroundLoader> instance{ new BackgroundLoader };
	return *instance;
}

void BackgroundLoader::Initialize() {
	GetInstance().initialize();
}

void BackgroundLoader::RegisterLoadQue(LoadEvent eventID, const std::string& filePath, const std::string& fileName) noexcept(false) {
	// mutexのlock
	std::lock_guard<std::mutex> lock{ referenceMutex };
	GetInstance().isLoading = true;
	// ロードイベント
	switch (eventID) {
	case LoadEvent::LoadTexture:
		GetInstance().loadEvents.emplace_back(
			eventID,
			std::make_unique<LoadingQue>(filePath, fileName, LoadingQue::LoadTextureData{ std::make_shared<Texture>(), nullptr })
		);
		break;
	case LoadEvent::LoadPolygonMesh:
		GetInstance().loadEvents.emplace_back(
			eventID,
			std::make_unique<LoadingQue>(filePath, fileName, LoadingQue::LoadPolygonMeshData{ std::make_shared<PolygonMesh>() }));
		break;
	case LoadEvent::LoadAudio:
		GetInstance().loadEvents.emplace_back(
			eventID,
			std::make_unique<LoadingQue>(filePath, fileName, LoadingQue::LoadAudioData{ std::make_shared<AudioResource>() }));
		break;
	default:
		Log("[BackgroundLoader] EventID is wrong.\n");
		break;
	}
	// 条件変数通知
	loadConditionVariable.notify_all();
}

void BackgroundLoader::WaitEndExecute() {
	std::unique_lock<std::mutex> uniqueLock{ executeMutex };
	// 実行が終わるまで待機
	waitConditionVariable.wait(uniqueLock, [] { return !IsLoading(); });
}

bool BackgroundLoader::IsLoading() noexcept {
	return GetInstance().isLoading;
}

void BackgroundLoader::initialize() noexcept(false) {
	isLoading = false;
	isEndProgram = false;
	// ロード用スレッド作成
	loadFunc = std::thread{ std::bind(&BackgroundLoader::load_manager, this) };
}

void BackgroundLoader::load_manager() {
	// プログラム自体の終了が行われない限りは続ける
	while (!isEndProgram) {
		// 条件変数用mutex
		std::unique_lock<std::mutex> lock{ referenceMutex };
		// loadEventが空ではない or プログラム終了通知が来ているまでwait
		loadConditionVariable.wait(lock, [] {return !GetInstance().loadEvents.empty() || GetInstance().isEndProgram; });

		// プログラム終了ならループを抜ける
		if (GetInstance().isEndProgram) {
			break;
		}

		// 先頭のイベントを取得(lock中なのでok)
		auto&& nowEvent = loadEvents.begin();
		// ここからはunlock
		lock.unlock();

		switch (nowEvent->eventId) {
		case LoadEvent::LoadTexture:
		{
			// テクスチャロードイベント
			auto& tex = std::get<0>(nowEvent->data->loadData);
			// テクスチャロード(intermediateResourceはコマンド実行に必要なので保存)
			tex.intermediateResource = tex.textureData->load_texture(nowEvent->data->filePath + "/" + nowEvent->data->fileName);
			// 先頭要素を転送キューに追加(内部要素のmoveなので、listそのものはmutex必要なし)
			waitLoadingQue.push_back(std::move(*nowEvent));
			break;
		}
		case LoadEvent::LoadPolygonMesh:
		{
			// メッシュロードイベント
			auto& mesh = std::get<1>(nowEvent->data->loadData);
			mesh.meshData->load(nowEvent->data->filePath, nowEvent->data->fileName);
			// 先頭要素を転送キューに追加(内部要素のmoveなので、listそのものはmutex必要なし)
			waitLoadingQue.emplace_back(std::move(*nowEvent));
		}
		break;
		case LoadEvent::LoadAudio:
		{
			// オーディオロードイベント
			auto& audio = std::get<2>(nowEvent->data->loadData);
			audio.audioData->load(nowEvent->data->filePath, nowEvent->data->fileName);
			// 先頭要素を転送キューに追加(内部要素のmoveなので、listそのものはmutex必要なし)
			waitLoadingQue.emplace_back(std::move(*nowEvent));
		}
		break;
		default:
			// デフォルトを通る場合はEventIDがおかしいので止める
			Log(std::format("[BackgroundLoader] EventID is wrong.\n\tID-\'{}\'\n\tFile-\'{}/{}\'\n\tIndex-\'{}\'\n",
				static_cast<int>(nowEvent->eventId),
				nowEvent->data->filePath,
				nowEvent->data->fileName,
				nowEvent->data->loadData.index(),
				nowEvent->data->loadData.valueless_by_exception()
			));
			std::range_error("[BackgroundLoader] EventID is wrong.");
			break;
		}

		// mutexの再ロック
		lock.lock();
		// listの先頭要素をpop
		loadEvents.pop_front();

		// 空だったら自動execute
		if (GetInstance().loadEvents.empty()) {
			// 実行イベント
			// コマンド実行
			DirectXCommand::ExecuteTextureCommand();
			// 終了まで待機
			DirectXCommand::WaitTextureCommand();
			// リセット
			DirectXCommand::ResetTextureCommand();
			// resourceViewの作成
			create_texture_view();
			transfer_data();
			// 実行状態をfalseに
			isLoading = false;
			// waitしているかもしれないので通知
			waitConditionVariable.notify_all();
		}
		// 自動ロック解除
	}
}

void BackgroundLoader::create_texture_view() {
	for (auto waitLoadingQueItr = waitLoadingQue.begin(); waitLoadingQueItr != waitLoadingQue.end(); ++waitLoadingQueItr) {
		// ロードイベントIDがTextureなら
		if (waitLoadingQueItr->eventId == LoadEvent::LoadTexture) {
			LoadingQue::LoadTextureData& tex = std::get<0>(waitLoadingQueItr->data->loadData);
			// ビューの作成
			tex.textureData->create_resource_view();
		}
	}
}

void BackgroundLoader::transfer_data() {
	// 新規データを転送
	for (auto waitLoadingQueItr = waitLoadingQue.begin(); waitLoadingQueItr != waitLoadingQue.end(); ++waitLoadingQueItr) {
		switch (waitLoadingQueItr->eventId) {
		case LoadEvent::LoadTexture:
		{
			LoadingQue::LoadTextureData& tex = std::get<0>(waitLoadingQueItr->data->loadData);
			// TextureManagerに転送
			TextureManager::Transfer(waitLoadingQueItr->data->fileName, tex.textureData);
			break;
		}
		case LoadEvent::LoadPolygonMesh:
		{
			LoadingQue::LoadPolygonMeshData& mesh = std::get<1>(waitLoadingQueItr->data->loadData);
			// PolygomMeshManagerに転送
			PolygonMeshManager::Transfer(waitLoadingQueItr->data->fileName, mesh.meshData);
			break;
		}
		case LoadEvent::LoadAudio:
		{
			LoadingQue::LoadAudioData& audio = std::get<2>(waitLoadingQueItr->data->loadData);
			// AudioManagerに転送
			AudioManager::Transfer(waitLoadingQueItr->data->fileName, audio.audioData);
			break;
		}
		default:
			break;
		}
	}
	// 全て移したのでクリア
	waitLoadingQue.clear();
}
