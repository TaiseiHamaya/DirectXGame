#include "call_once.h"

bool eps::call_once::operator()(std::function<bool()> function) {
	if (!function) {
		return false;
	}
	if (isTriggered) {
		return false;
	}
	isTriggered = function();
	return isTriggered;
}

void eps::call_once::reset() noexcept {
	isTriggered = false;
}
