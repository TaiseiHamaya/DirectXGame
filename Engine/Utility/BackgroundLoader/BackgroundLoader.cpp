#include "BackgroundLoader.h"

#include <cassert>
#include <functional>
#include <mutex>

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/Texture.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/TextureManager/TextureManager.h"
#include "Engine/GameObject/PolygonMesh/PolygonMesh.h"
#include "Engine/GameObject/PolygonMesh/PolygonMeshManager/PolygonMeshManager.h"

std::mutex executeMutex;
std::mutex referenceMutex;
std::condition_variable waitConditionVariable;
std::condition_variable loadConditionVariable;

BackgroundLoader::BackgroundLoader() = default;

BackgroundLoader::~BackgroundLoader() {
	isEndProgram = true;
	if (loadFunc.joinable()) {
		loadConditionVariable.notify_all();
		loadFunc.join();
	}
}

BackgroundLoader& BackgroundLoader::GetInstance() {
	static std::unique_ptr<BackgroundLoader> instance{ new BackgroundLoader };
	return *instance;
}

void BackgroundLoader::Initialize() {
	GetInstance().initialize();
}

void BackgroundLoader::RegisterLoadQue(LoadEvent eventID, const std::string& filePath, const std::string& textureName) {
	// mutexのlock
	std::lock_guard<std::mutex> lock(referenceMutex);
	// ロードイベント
	switch (eventID) {
	case LoadEvent::LoadTexture:
		GetInstance().loadEvents.emplace_back(
			eventID,
			std::make_unique<LoadingQue>(filePath, textureName, LoadingQue::LoadTextureData{ std::make_shared<Texture>(), nullptr })
		);
		break;
	case LoadEvent::LoadPolygonMesh:
		GetInstance().loadEvents.emplace_back(
			eventID,
			std::make_unique<LoadingQue>(filePath, textureName, LoadingQue::LoadPolygonMeshData{ std::make_shared<PolygonMesh>() }));
		break;
	default:
		break;
	}
	// 条件変数通知
	loadConditionVariable.notify_all();
}

void BackgroundLoader::WaitEndExecute() {
	std::unique_lock<std::mutex> uniqueLock(executeMutex);
	// 実行が終わるまで待機
	waitConditionVariable.wait(uniqueLock, [] { return !IsLoading() && GetInstance().loadEvents.empty(); });
}

bool BackgroundLoader::IsLoading() {
	return GetInstance().isExecuting;
}

void BackgroundLoader::initialize() {
	isExecuting = false;
	isEndProgram = false;
	// ロード用スレッド作成
	loadFunc = std::thread{ std::bind(&BackgroundLoader::load_manager, this) };
}

void BackgroundLoader::load_manager() {
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
			LoadingQue::LoadTextureData& tex = std::get<0>(nowEvent->data->loadData);
			// データ追加イベント
			// テクスチャロード(intermediateResourceはコマンド実行に必要なので保存)
			tex.intermediateResource = tex.textureData->load_texture(nowEvent->data->filePath + "/" + nowEvent->data->fileName);
			// 先頭要素をResourceView作成キューに追加(内部要素のmoveなので、listそのものはmutex必要なし)
			waitLoadingQue.push_back(std::move(loadEvents.front()));
			break;
		}
		case LoadEvent::LoadPolygonMesh:
		{
			LoadingQue::LoadPolygonMeshData& mesh = std::get<1>(nowEvent->data->loadData);
			mesh.meshData->load(nowEvent->data->filePath, nowEvent->data->fileName);
			waitLoadingQue.emplace_back(std::move(loadEvents.front()));
		}
		break;
		default:
			// デフォルトに通る場合はEventIDがおかしいので止める
			assert("EventID is wrong");
			break;
		}
		// ロックして
		lock.lock();
		// listの先頭要素をpop
		loadEvents.pop_front();
		// 空だったら自動execute
		if (GetInstance().loadEvents.empty()) {
			isExecuting = true;
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
			isExecuting = false;
			// waitしているかもしれないので通知
			waitConditionVariable.notify_all();
		}
	}
}

void BackgroundLoader::create_texture_view() {
	for (auto waitLoadingQueItr = waitLoadingQue.begin(); waitLoadingQueItr != waitLoadingQue.end(); ++waitLoadingQueItr) {
		if (waitLoadingQueItr->eventId == LoadEvent::LoadTexture) {
			LoadingQue::LoadTextureData& tex = std::get<0>(waitLoadingQueItr->data->loadData);
			tex.textureData->create_resource_view();
		}
	}
}

void BackgroundLoader::transfer_data() {
	for (auto waitLoadingQueItr = waitLoadingQue.begin(); waitLoadingQueItr != waitLoadingQue.end(); ++waitLoadingQueItr) {
		switch (waitLoadingQueItr->eventId) {
		case LoadEvent::LoadTexture:
		{
			LoadingQue::LoadTextureData& tex = std::get<0>(waitLoadingQueItr->data->loadData);
			TextureManager::Transfer(waitLoadingQueItr->data->fileName, tex.textureData);
			break;
		}
		case LoadEvent::LoadPolygonMesh:
		{
			LoadingQue::LoadPolygonMeshData& mesh = std::get<1>(waitLoadingQueItr->data->loadData);
			PolygonMeshManager::Transfer(waitLoadingQueItr->data->fileName, mesh.meshData);
			break;
		}
		default:
			break;
		}
	}
	// 全て移したのでクリア
	waitLoadingQue.clear();
}
