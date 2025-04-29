#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <list>
#include <utility>

class WorldClockProfiler {
public:
	void update();
	void debug_gui() const;

private:
	r32 timeSummation{ 0 };
	r32 fpsSummation{ 0 };
	r32 averageFPS{ 0 };
	std::list<std::pair<r32, u64>> frameTimeInfomation;
};

#endif // _DEBUG
