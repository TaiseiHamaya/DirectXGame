#include "TextureManager.h"

#include <functional>
#include <mutex>
#include <condition_variable>
#include <cassert>

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/Texture.h"

TextureManager::TextureManager() = default;
std::mutex executeMutex;
std::mutex referenceMutex;
std::condition_variable waitConditionVariable;
std::condition_variable loadConditionVariable;

TextureManager::~TextureManager() {
	isEndProgram = true;
	if (loadFunc.joinable()) {
		loadConditionVariable.notify_all();
		loadFunc.join();
	}
};

TextureManager& TextureManager::GetInstance() {
	static std::unique_ptr<TextureManager> instance{ new TextureManager };
	return *instance;
}

void TextureManager::Initialize() {
	GetInstance().initialize();
}

void TextureManager::RegisterLoadQue(const std::string& filePath, const std::string& textureName) {
	if (GetInstance().textureRegisteredList.find(textureName) != GetInstance().textureRegisteredList.end()) {
		return;
	}
	// mutexのlock
	std::lock_guard<std::mutex> lock(referenceMutex);
	// ロードイベント
	GetInstance().loadEvents.push_back({ LoadEvent::EmplaceData, std::make_unique<LoadingQue>(filePath, textureName, LoadStatus::InQueue, std::make_shared<Texture>()) });
	// 条件変数通知
	loadConditionVariable.notify_all();
}

void TextureManager::LoadImperative() {
	GetInstance().isExecuting = true;
	// mutexのlock
	std::lock_guard<std::mutex> lock(referenceMutex);
	// executeのイベント
	GetInstance().loadEvents.push_back({ LoadEvent::Execute, nullptr });
	// 条件変数通知
	loadConditionVariable.notify_all();
}

void TextureManager::WaitEndExecute() {
	std::unique_lock<std::mutex> uniqueLock(executeMutex);
	// 実行が終わるまで待機
	waitConditionVariable.wait(uniqueLock, [] {return !GetInstance().isExecuting; });
}

void TextureManager::LoadImperativeAndWait() {
	// どっちも同時にやる
	LoadImperative();
	WaitEndExecute();
}

std::weak_ptr<Texture> TextureManager::GetTexture(const std::string& textureName) {
	std::lock_guard<std::mutex> lock(referenceMutex);
	// 存在しないならエラー
	assert(GetInstance().textureRegisteredList.find(textureName) != GetInstance().textureRegisteredList.end());
	// 見つかったらそのデータのweak_ptrを返す
	return GetInstance().textureInstanceList.at(textureName);
}

void TextureManager::initialize() {
	isExecuting = false;
	isEndProgram = false;
	// ロード用スレッド作成
	loadFunc = std::thread{ std::bind(&TextureManager::load_manager, this) };
}

void TextureManager::load_manager() {
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
		case LoadEvent::Execute:
			// 実行イベント
			// コマンド実行
			DirectXCommand::ExecuteTextureCommand();
			// 終了まで待機
			DirectXCommand::WaitTextureCommand();
			// リセット
			DirectXCommand::ResetTextureCommand();
			// resourceViewの作成
			create_view();
			// 実行状態をfalseに
			isExecuting = false;
			// waitしているかもしれないので通知
			waitConditionVariable.notify_all();
			break;
		case LoadEvent::EmplaceData:
			// データ追加イベント
			// テクスチャロード(intermediateResourceはコマンド実行に必要なので保存)
			nowEvent->que->intermediateResource = nowEvent->que->texture->load_texture(nowEvent->que->filePath + "/" + nowEvent->que->fileName);
			// ステータスをコマンドセット状態に変更。
			nowEvent->que->status = LoadStatus::SetCommand;
			// 先頭要素をResourceView作成キューに追加(内部要素のmoveなので、listそのものはmutex必要なし)
			waitLoadingQue.push_back(std::move(loadEvents.front()));
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
	}
}

void TextureManager::create_view() {
	for (auto itr = waitLoadingQue.begin(); itr != waitLoadingQue.end(); ++itr) {
		// resourceViewを作成
		itr->que->texture->create_resource_view();
		// ロック
		std::lock_guard<std::mutex> lock(referenceMutex);
		// 参照用テクスチャデータに要素追加
		textureInstanceList.emplace(itr->que->fileName, itr->que->texture);
		textureRegisteredList.emplace(itr->que->fileName);
	}
	// 全て移したのでクリア
	waitLoadingQue.clear();
}
