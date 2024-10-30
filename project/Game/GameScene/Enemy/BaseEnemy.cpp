#include "BaseEnemy.h"

#include <Engine/Runtime/WorldClock/WorldClock.h>

void BaseEnemy::initialize() {
}

void BaseEnemy::update() {
	timer += WorldClock::DeltaSeconds();
	if (timer >= ActiveTime) {
		isActive = false;
	}
}
