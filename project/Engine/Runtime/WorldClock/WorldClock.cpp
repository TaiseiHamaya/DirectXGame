#include "WorldClock.h"

#include <ratio>
#include <thread>

#include "Engine/Application/EngineSettings.h"

namespace chrono = std::chrono;

void WorldClock::Initialize() {
	auto& instance = GetInstance();
	instance.startFrameTimePoint = chrono::high_resolution_clock::now();
	std::this_thread::sleep_for(chrono::microseconds(1666));
	instance.frameTimeInfomation = {};
	instance.fpsSummation = 0;
	instance.deltaSeconds = EngineSettings::FixDeltaSeconds;
	instance.timeSummation = 0;
#ifdef _DEBUG
	instance.isFixDeltaTime = false;
#endif // _DEBUG
}

void WorldClock::Update() {
	// 少数型秒のusing
	using second_f = chrono::duration<float, std::ratio<1>>;

	auto&& instance = GetInstance();

	// 現在時刻を取得
	auto now = chrono::high_resolution_clock::now();
	// duration算出
	auto secDuration = chrono::duration_cast<second_f>(now - instance.startFrameTimePoint);
	// deltaTimeとして記録
#ifdef _DEBUG
	instance.deltaSeconds = instance.isFixDeltaTime ? std::min(EngineSettings::FixDeltaSeconds, secDuration.count()) : secDuration.count();
#else
	instance.deltaSeconds = secDuration.count();
#endif // _DEBUG

	// 平均フレーム秒を算出
	// リストに追加
	auto& newFrameInfo = instance.frameTimeInfomation.emplace_back(instance.deltaSeconds, 0);
	// 追加分だけ総和にも追加
	instance.timeSummation += instance.deltaSeconds;
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
		newFrameInfo.second = instance.frameTimeInfomation.size();
		// その分だけSumを増やす
		instance.fpsSummation += newFrameInfo.second;

		// 平均フレームレートを算出
		instance.averageFPS = instance.fpsSummation / instance.frameTimeInfomation.size();
	}
	// Startを更新
	instance.startFrameTimePoint = now;
}

float WorldClock::DeltaSeconds() {
	return GetInstance().deltaSeconds;
}

float WorldClock::AverageFPS() {
	return GetInstance().averageFPS;
}

const chrono::high_resolution_clock::time_point& WorldClock::BeginTime() {
	return GetInstance().startFrameTimePoint;
}

#ifdef _DEBUG
void WorldClock::IsFixDeltaTime(bool boolean) {
	GetInstance().isFixDeltaTime = boolean;
}

#include <imgui.h>
void WorldClock::DebugGui() {
	auto&& instance = GetInstance();
	float deltaMS = instance.deltaSeconds * 1000.0f;
	uint32_t msInteger = static_cast<uint32_t>(deltaMS);
	uint32_t msDecimal = static_cast<uint32_t>((deltaMS - std::floor(deltaMS)) * 1e4f);
	ImGui::Text(std::format("Delta : {:>5}.{:0>4}ms", msInteger, msDecimal).c_str());
	ImGui::Text("AverageFPS : %.1fFPS", instance.averageFPS);
	ImGui::Checkbox("IsFixDeltaTime", &instance.isFixDeltaTime);
	ImGui::Checkbox("IsUnlimitedFPS", &instance.isUnlimitedRefreshRate);
}
#endif // _DEBUG
