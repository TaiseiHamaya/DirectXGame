#include "WorldTimer.h"

#include "./WorldClock.h"

void WorldTimer::ahead() {
	timer += WorldClock::DeltaSeconds();
}

void WorldTimer::back() {
	timer -= WorldClock::DeltaSeconds();
}

void WorldTimer::set(float time) {
	timer = 0;
}

float WorldTimer::time() const {
	return timer;
}
