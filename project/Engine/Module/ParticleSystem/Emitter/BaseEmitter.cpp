#include "BaseEmitter.h"

#include <Engine/Application/WorldClock/WorldClock.h>

void BaseEmitter::update() {
	timer += WorldClock::DeltaSeconds();
	if (isLoop && is_end()) {
		restart();
	}
}

bool BaseEmitter::is_end() const {
	return timer >= duration;
}
