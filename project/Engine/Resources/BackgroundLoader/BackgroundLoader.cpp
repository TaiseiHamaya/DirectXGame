#include "BackgroundLoader.h"

#include <functional>
#include <mutex>

#include "Engine/Debug/Output.h"
#include "Engine/Rendering/DirectX/DirectXCommand/DirectXCommand.h"

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
	static BackgroundLoader instance{};
	return instance;
}

void BackgroundLoader::Initialize() {
	GetInstance().initialize();
}

void BackgroundLoader::RegisterLoadQue(std::unique_ptr<BaseResourceBuilder> builder) noexcept(false) {
	// mutexのlock
	std::lock_guard<std::mutex> lock{ referenceMutex };
	auto& instance = GetInstance();
	instance.isLoading = true;

	builder->preprocess();
	instance.loadEvents.emplace_back(std::move(builder));

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
		loadConditionVariable.wait(lock, [&] {return !loadEvents.empty() || isEndProgram; });

		// プログラム終了ならループを抜ける
		if (isEndProgram) {
			break;
		}

		// 先頭のイベントを取得(lock中なのでok)
		auto&& nowEvent = loadEvents.begin();
		// ここからはunlock
		lock.unlock();

		// ----- ロードプロセス -----
		bool result = false;
		result = nowEvent->data->run();

		// 成功した場合は転送キューにへ
		if (result) {
			// 先頭要素を転送キューに追加(内部要素のmoveなので、listそのものはmutex必要なし)
			waitTransferQue.emplace_back(std::move(*nowEvent));
		}
		// 失敗時は転送せずエラー出力
		//else {
		//	Console("Failed loading. File-\'{}\'\n", nowEvent->data->filepath().string());
		//}

		// mutexの再ロック
		lock.lock();
		// listの先頭要素をpop
		loadEvents.pop_front();

		// 空だったら自動execute
		if (loadEvents.empty()) {
			Console("Notification : Load events is empty. Start uploading texture.\n");
			// ----- GPUコマンドの実行 -----
			// コマンド実行
			DirectXCommand::ExecuteTextureCommand();
			// 終了まで待機
			DirectXCommand::WaitTextureCommand();
			// リセット
			DirectXCommand::ResetTextureCommand();
			Console("Succeeded.\n");

			// ----- 実行済みを転送 -----
			// 直前にやる
			postprocess();
			// 転送
			transfer_all();
			// 実行状態をfalseに
			isLoading = false;
			// waitしているかもしれないので通知
			waitConditionVariable.notify_all();
		}
		// 自動ロック解除
	}
}

void BackgroundLoader::postprocess() {
	for (EventList& waitLoading : waitTransferQue) {
		waitLoading.data->postprocess();
	}
}

void BackgroundLoader::transfer_all() {
	for (EventList& waitLoading : waitTransferQue) {
		waitLoading.data->transfer();
	}
	// 転送完了したので削除
	waitTransferQue.clear();
}
