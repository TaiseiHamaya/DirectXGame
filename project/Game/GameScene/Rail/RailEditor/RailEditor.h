#pragma once

#ifdef _DEBUG
class Rail;

class RailEditor {


public:
	void debug_gui();

private:
	Rail* rail;
};
#endif // _DEBUG
