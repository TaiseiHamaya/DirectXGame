#pragma once

#ifdef _DEBUG

#include <vector>

class Rail;
struct RailPoint;

class RailEditor {


public:
	void initialize(Rail* rail_);
	void debug_gui();

private:
	void apply();
	void export_json();

private:
	Rail* rail;

	std::vector<RailPoint> editRailPoints;
};
#endif // _DEBUG
