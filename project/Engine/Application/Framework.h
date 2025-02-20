#pragma once

#include "Engine/Debug/Profiler/TimestampProfiler.h"

class Framework {
public:
	Framework() = default;
	virtual ~Framework() = default;

protected:
	virtual void initialize();
	virtual void finalize();
	void begin_frame();
	virtual void update();
	virtual void draw() const;
	void end_frame();

public:
	void run();

#ifdef _DEBUG
private:
	mutable TimestampProfiler profiler;
#endif // _DEBUG
};
