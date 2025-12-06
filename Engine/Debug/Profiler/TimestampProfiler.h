#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <chrono>
#include <string>
#include <optional>

namespace szg {

class TimestampProfiler {
private:
	using clock = std::chrono::high_resolution_clock;

private:
	struct Timestamp {
		std::string name;
		clock::time_point time;
		std::optional<std::chrono::microseconds> duration;
	};

public:
	void debug_gui();

	void clear_timestamps();
	void timestamp(const std::string& name);

private:
	std::vector<Timestamp> timestamps;
};

}; // szg

#endif // _DEBUG
