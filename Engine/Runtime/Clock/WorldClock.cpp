#include "WorldClock.h"

#include <ratio>
#include <thread>

#include "Engine/Application/EngineSettings.h"

namespace chrono = std::chrono;

void WorldClock::Initialize() {
	auto& instance = GetInstance();
	instance.startFrameTimePoint = chrono::high_resolution_clock::now();
	std::this_thread::sleep_for(chrono::microseconds(1666));
	instance.deltaSeconds = EngineSettings::FixDeltaSeconds;
}

void WorldClock::Update() {
	// 少数型秒のusing
	using second_f = chrono::duration<r32, std::ratio<1>>;

	auto&& instance = GetInstance();

	// 現在時刻を取得
	auto now = chrono::high_resolution_clock::now();
	// duration算出
	auto secDuration = chrono::duration_cast<second_f>(now - instance.startFrameTimePoint);
	// deltaTimeとして記録
	instance.deltaSeconds = EngineSettings::IsFixDeltaTime ? std::min(EngineSettings::FixDeltaSeconds, secDuration.count()) : secDuration.count();

	// Startを更新
	instance.startFrameTimePoint = now;

#ifdef DEBUG_FEATURES_ENABLE
	instance.profiler.update();
#endif // _DEBUG
}

r32 WorldClock::DeltaSeconds() {
	return GetInstance().deltaSeconds;
}

const chrono::high_resolution_clock::time_point& WorldClock::BeginTime() {
	return GetInstance().startFrameTimePoint;
}

#ifdef DEBUG_FEATURES_ENABLE
#include <imgui.h>
void WorldClock::DebugGui() {
	auto&& instance = GetInstance();
	r32 deltaMS = instance.deltaSeconds * 1000.0f;
	u32 msInteger = static_cast<u32>(deltaMS);
	u32 msDecimal = static_cast<u32>((deltaMS - std::floor(deltaMS)) * 1e4f);
	ImGui::Text(std::format("Delta : {:>5}.{:0>4}ms", msInteger, msDecimal).c_str());
	instance.profiler.debug_gui();
	ImGui::Checkbox("IsFixDeltaTime", &EngineSettings::IsFixDeltaTime);
	ImGui::Checkbox("IsUnlimitedFPS", &EngineSettings::IsUnlimitedFPS);
}
#endif // _DEBUG
