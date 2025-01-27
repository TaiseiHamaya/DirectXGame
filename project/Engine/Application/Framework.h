#pragma once

class Framework {
public:
	Framework() = default;
	virtual ~Framework() = default;

public:
	virtual void initialize();
	virtual void finalize();
	void begin_frame();
	virtual void update();
	virtual void draw() const;
	void end_frame();

public:
	void run();
};
