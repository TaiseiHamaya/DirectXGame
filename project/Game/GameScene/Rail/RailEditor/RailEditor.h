#pragma once

#ifdef _DEBUG

#include <list>
#include <vector>

class Rail;
class MeshInstance;
struct RailPoint;

class RailEditor {


public:
	void initialize(Rail* rail_);
	void debug_gui();
	void debug_draw();

private:
	void apply();
	void export_json();

private:
	Rail* rail;

	std::list<RailPoint> editRailPoints;
	std::vector<MeshInstance> editDraw;
};
#endif // _DEBUG
