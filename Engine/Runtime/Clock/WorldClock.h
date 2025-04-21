#pragma once

#include <chrono>

#include <Library/Utility/Template/SingletonInterface.h>

#ifdef DEBUG_FEATURES_ENABLE
#include "Engine/Debug/Profiler/WorldClockProfiler.h"
#endif // _DEBUG

class WorldClock final : public SingletonInterface<WorldClock> {
	__CLASS_SINGLETON_INTERFACE(WorldClock)

public:
	static void Initialize();
	static void Update();

public:
	static float DeltaSeconds();
	static const std::chrono::high_resolution_clock::time_point& BeginTime();

#ifdef DEBUG_FEATURES_ENABLE
public:
	static void DebugGui();
#endif // _DEBUG

private:
	float deltaSeconds{ 0.0f };
	std::chrono::high_resolution_clock::time_point startFrameTimePoint;

#ifdef DEBUG_FEATURES_ENABLE
	WorldClockProfiler profiler;
#endif // _DEBUG
};
