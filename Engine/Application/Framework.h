#pragma once

class Framework {
public:
	Framework() = default;
	virtual ~Framework() = default;

public:
	virtual void initialize() = 0;

	void run();
};
