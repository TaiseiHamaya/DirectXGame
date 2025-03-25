#include "WorldClockProfiler.h"

#ifdef DEBUG_FEATURES_ENABLE

#include <Engine/Runtime/Clock/WorldClock.h>

#include <imgui.h>

void WorldClockProfiler::update() {
	// 平均フレーム秒を算出
	// リストに追加
	auto& newFrameInfo = frameTimeInfomation.emplace_back(WorldClock::DeltaSeconds(), 0);
	// 追加分だけ総和にも追加
	timeSummation += WorldClock::DeltaSeconds();
	// 1以上の場合
	while (timeSummation > 1.0f) {
		auto& deleteValue = frameTimeInfomation.front();
		// 1未満になるように減算し、リストからも削除
		timeSummation -= deleteValue.first;
		// fpsSumも減らす
		fpsSummation -= deleteValue.second;
		frameTimeInfomation.pop_front();
	}
	if (frameTimeInfomation.empty()) {
		averageFPS = 0;
	}
	else {
		// 今の容量を算出
		newFrameInfo.second = frameTimeInfomation.size();
		// その分だけSumを増やす
		fpsSummation += newFrameInfo.second;

		// 平均フレームレートを算出
		averageFPS = fpsSummation / frameTimeInfomation.size();
	}
}

void WorldClockProfiler::debug_gui() {
	ImGui::Text("AverageFPS : %.1fFPS", averageFPS);
}

#endif // _DEBUG
