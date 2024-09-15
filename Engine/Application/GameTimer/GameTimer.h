#pragma once

#include <chrono>
#include <list>

class GameTimer {
private:
	GameTimer() = default;

public:
	~GameTimer() = default;

	GameTimer(const GameTimer&) = delete;
	GameTimer& operator=(const GameTimer&) = delete;

public:
	static GameTimer& GetInstance();

	static void Initialize();
	static void Update();

public:
	static float DeltaTime();
	static float AverageFPS();
	static const std::chrono::system_clock::time_point& BeginTime();
	static void IsFixDeltaTime(bool boolean);

#ifdef _DEBUG
	static void DebugGui();
#endif // _DEBUG

private:
	float deltaTime;
	std::chrono::system_clock::time_point startFrameTimePoint;

	std::list<std::pair<float, size_t>> frameTimeInfomation;
	float timeSummation;
	float fpsSummation;
	float averageFPS;

	bool isFixDeltaTime;
};

