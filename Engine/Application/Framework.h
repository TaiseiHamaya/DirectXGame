#pragma once

namespace szg {

class Framework {
public:
	Framework() = default;
	virtual ~Framework() = default;

public:
	virtual void initialize() = 0;

	void run();
};

}; // szg
