#include "GameTimer.h"

#include <ratio>

namespace chrono = std::chrono;

GameTimer& GameTimer::GetInstance() {
	static GameTimer instance{};
	return instance;
}

void GameTimer::Initialize() {
	auto& instance = GetInstance();
	instance.startFrameTimePoint = chrono::system_clock::now();
	instance.fps = 60;
	instance.deltaTimeList = {};
	instance.timeSummation = 0;
}

void GameTimer::Update() {
	// 少数型秒のusing
	using second_f = std::chrono::duration<float, std::ratio<1, 1>>;

	auto&& instance = GetInstance();

	// 現在時刻を取得
	auto now = chrono::system_clock::now();
	// duration算出
	auto secDuration = chrono::duration_cast<second_f>(now - instance.startFrameTimePoint);
	// deltaTimeとして記録
	instance.deltaTime = instance.isFixDeltaTime ? std::min((1.0f / 60), secDuration.count()) : secDuration.count();
	// リストに追加
	instance.deltaTimeList.emplace_back(instance.deltaTime);
	// 追加分だけ総和にも追加
	instance.timeSummation += instance.deltaTime;
	// 1以上の場合
	while (instance.timeSummation > 1.0f) {
		// 1未満になるように減算し、リストからも削除
		instance.timeSummation -= instance.deltaTimeList.front();
		instance.deltaTimeList.pop_front();
	}
	// 平均算出
	instance.fps = static_cast<float>(instance.deltaTimeList.size());
	// Startを更新
	instance.startFrameTimePoint = now;
}

float GameTimer::DeltaTime() {
	return GetInstance().deltaTime;
}

float GameTimer::AverageFPS() {
	return GetInstance().fps;
}

const std::chrono::system_clock::time_point& GameTimer::BeginTime() {
	return GetInstance().startFrameTimePoint;
}

void GameTimer::IsFixDeltaTime(bool boolean) {
	GetInstance().isFixDeltaTime = boolean;
}
