#pragma once

#ifdef _DEBUG

#include <chrono>
#include <list>

class WorldClockProfiler {
public:
	void update();
	void debug_gui();

private:
	float timeSummation{ 0 };
	float fpsSummation{ 0 };
	float averageFPS{ 0 };
	std::list<std::pair<float, size_t>> frameTimeInfomation;
};

#endif // _DEBUG
