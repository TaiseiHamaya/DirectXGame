#include "EmitterSample.h"

#include "Engine/Application/WorldClock/WorldClock.h"

void EmitterSample::initialize() {
	isLoop = true;
	duration = 3;
	numEmits = 3;
	isEmit = false;
}

void EmitterSample::update() {
	isEmit = false;
	BaseEmitter::update();
	if (std::fmod(timer, 1.0f) < WorldClock::DeltaSeconds()) {
		isEmit = true;
	}
}

void EmitterSample::restart() {
	timer = 0;
}
