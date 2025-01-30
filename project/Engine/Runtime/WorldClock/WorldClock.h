#pragma once

#include <chrono>
#include <list>

#include <Library/Utility/Template/SingletonInterface.h>

class WorldClock final : public SingletonInterface<WorldClock> {
	__SINGLETON_INTERFACE(WorldClock)

public:
	static void Initialize();
	static void Update();

public:
	static float DeltaSeconds();
	static float AverageFPS();
	static const std::chrono::high_resolution_clock::time_point& BeginTime();

#ifdef _DEBUG
public:
	static void IsFixDeltaTime(bool boolean);
	static void DebugGui();
	static bool IsUnlimitedFPS() { return GetInstance().isUnlimitedRefreshRate; };
#endif // _DEBUG

private:
	float deltaSeconds;
	std::chrono::high_resolution_clock::time_point startFrameTimePoint;

	std::list<std::pair<float, size_t>> frameTimeInfomation;
	float timeSummation;
	float fpsSummation;
	float averageFPS;

#ifdef _DEBUG
	bool isFixDeltaTime;
	bool isUnlimitedRefreshRate;
#endif // _DEBUG
};

