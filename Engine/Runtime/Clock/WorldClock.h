#pragma once

#include <chrono>

#include <Library/Utility/Template/SingletonInterface.h>

#ifdef DEBUG_FEATURES_ENABLE
#include "Engine/Debug/Profiler/WorldClockProfiler.h"
#endif // _DEBUG

namespace szg {

class WorldClock final : public SingletonInterface<WorldClock> {
	SZG_CLASS_SINGLETON(WorldClock)

public:
	static void Initialize();
	static void Update();

public:
	static r32 DeltaSeconds();
	static const std::chrono::high_resolution_clock::time_point& BeginTime();

#ifdef DEBUG_FEATURES_ENABLE
public:
	static void DebugGui();
#endif // _DEBUG

private:
	r32 deltaSeconds{ 0.0f };
	std::chrono::high_resolution_clock::time_point startFrameTimePoint;

#ifdef DEBUG_FEATURES_ENABLE
	WorldClockProfiler profiler;
#endif // _DEBUG
};

}; // szg
