#include "GameTimer.h"

#include <ratio>

namespace chrono = std::chrono;

GameTimer& GameTimer::GetInstance() {
	static GameTimer instance{};
	return instance;
}

void GameTimer::Initialize() {
	GetInstance().preFrameTime = chrono::system_clock::now();
}

void GameTimer::Update() {
	// 少数型秒のusing
	using second_f = std::chrono::duration<float, std::ratio<1, 1>>;

	auto&& instance = GetInstance();

	// 現在時刻を取得
	auto now = chrono::system_clock::now();
	// duration算出
	auto secDuration = chrono::duration_cast<second_f>(now - instance.preFrameTime);
	// deltaTimeとして記録
	instance.deltaTime = instance.isFixDeltaTime ? std::min((1.0f / 60), secDuration.count()) : secDuration.count();

	// preを更新
	instance.preFrameTime = std::move(now);
}

float GameTimer::DeltaTime() {
	return GetInstance().deltaTime;
}

void GameTimer::IsFixDeltaTime(bool boolean) {
	GetInstance().isFixDeltaTime = boolean;
}
