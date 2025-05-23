#include "WorldTimer.h"

#include "./WorldClock.h"

void WorldTimer::ahead() {
	timer += WorldClock::DeltaSeconds();
}

void WorldTimer::back() {
	timer -= WorldClock::DeltaSeconds();
}

void WorldTimer::set(r32 time) {
	timer = 0;
}

r32 WorldTimer::time() const {
	return timer;
}
