#include "GameTimer.h"

#include <ratio>
#include <thread>

namespace chrono = std::chrono;

GameTimer& GameTimer::GetInstance() {
	static GameTimer instance{};
	return instance;
}

void GameTimer::Initialize() {
	auto& instance = GetInstance();
	instance.startFrameTimePoint = chrono::high_resolution_clock::now();
	std::this_thread::sleep_for(std::chrono::microseconds(16667));
	instance.frameTimeInfomation = {};
	instance.fpsSummation = 0;
	instance.deltaTime = 1.0f / 60.0f;
	instance.timeSummation = 0;
#ifdef _DEBUG
	instance.isFixDeltaTime = false;
#endif // _DEBUG
}

void GameTimer::Update() {
	// 少数型秒のusing
	using second_f = std::chrono::duration<float, std::ratio<1, 1>>;

	auto&& instance = GetInstance();

	// 現在時刻を取得
	auto now = chrono::high_resolution_clock::now();
	// duration算出
	auto secDuration = chrono::duration_cast<second_f>(now - instance.startFrameTimePoint);
	// deltaTimeとして記録
#ifdef _DEBUG
	instance.deltaTime = instance.isFixDeltaTime ? std::min((1.0f / 60), secDuration.count()) : secDuration.count();
#else
	instance.deltaTime = secDuration.count();
#endif // _DEBUG


	// 平均フレーム秒を算出
	// リストに追加
	auto& newFrameInfo = instance.frameTimeInfomation.emplace_back(instance.deltaTime, 0);
	// 追加分だけ総和にも追加
	instance.timeSummation += instance.deltaTime;
	// 1以上の場合
	while (instance.timeSummation > 1.0f) {
		auto& deleteValue = instance.frameTimeInfomation.front();
		// 1未満になるように減算し、リストからも削除
		instance.timeSummation -= deleteValue.first;
		// fpsSumも減らす
		instance.fpsSummation -= deleteValue.second;
		instance.frameTimeInfomation.pop_front();
	}
	if (instance.frameTimeInfomation.empty()) {
		instance.averageFPS = 0;
	}
	else {
		// 今の容量を算出
		newFrameInfo.second = (instance.frameTimeInfomation.size());
		// その分だけSumを増やす
		instance.fpsSummation += newFrameInfo.second;

		// 平均フレームレートを算出
		instance.averageFPS = instance.fpsSummation / instance.frameTimeInfomation.size();
	}
	// Startを更新
	instance.startFrameTimePoint = now;
}

float GameTimer::DeltaTime() {
	return GetInstance().deltaTime;
}

float GameTimer::AverageFPS() {
	return GetInstance().averageFPS;
}

const std::chrono::high_resolution_clock::time_point& GameTimer::BeginTime() {
	return GetInstance().startFrameTimePoint;
}

#ifdef _DEBUG
void GameTimer::IsFixDeltaTime(bool boolean) {
	GetInstance().isFixDeltaTime = boolean;
}

#include <imgui.h>
void GameTimer::DebugGui() {
	auto&& instance = GetInstance();
	ImGui::Text(std::format("DeltaTime : {:03.5}ms", instance.deltaTime * 1000.0f).c_str());
	ImGui::Text("AvarageFPS : %.1fFPS", instance.averageFPS);
	ImGui::Checkbox("IsFixDeltaTime", &instance.isFixDeltaTime);
	ImGui::Checkbox("IsUnlimitedFPS", &instance.isUnlimitedRefreshRate);

}
#endif // _DEBUG
