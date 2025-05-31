#include "WorldTimer.h"

#include "./WorldClock.h"

void WorldTimer::ahead() {
	timer += WorldClock::DeltaSeconds();
}

void WorldTimer::back() {
	timer -= WorldClock::DeltaSeconds();
}

void WorldTimer::set(r32 time) {
	timer = time;
}

r32 WorldTimer::time() const {
	return timer;
}

bool WorldTimer::just_crossed(r32 time) const noexcept {
	return timer >= time && timer - time <= WorldClock::DeltaSeconds();
}
