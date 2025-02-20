#pragma once

#include <chrono>

#include <Library/Utility/Template/SingletonInterface.h>

#ifdef _DEBUG
#include "Engine/Debug/Profiler/WorldClockProfiler.h"
#endif // _DEBUG

class WorldClock final : public SingletonInterface<WorldClock> {
	__SINGLETON_INTERFACE(WorldClock)

public:
	static void Initialize();
	static void Update();

public:
	static float DeltaSeconds();
	static const std::chrono::high_resolution_clock::time_point& BeginTime();

#ifdef _DEBUG
public:
	static void DebugGui();
#endif // _DEBUG

private:
	float deltaSeconds;
	std::chrono::high_resolution_clock::time_point startFrameTimePoint;

#ifdef _DEBUG
	WorldClockProfiler profiler;
#endif // _DEBUG
};

