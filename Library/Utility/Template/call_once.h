#pragma once

#include <functional>

namespace eps {

class call_once final {
public:
	bool operator()(std::function<bool()> function);
	void reset() noexcept;

private:
	bool isTriggered{ false };
};

};
