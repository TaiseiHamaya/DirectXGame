#include "EmitterSample.h"

#include <random>

namespace EmitterRandomSample {

static std::random_device device{};
static std::mt19937 mt{ device() };
static std::uniform_real_distribution<float> ufd10{ 0,10 };
static std::uniform_real_distribution<float> ufd1010{ -10,10 };
static std::uniform_real_distribution<float> ufd1{ 0,1 };

}

void EmitterSample::initialize() {
	isLoop = true;
	duration = 3;
	numEmits = 3;
	isEmit = false;
}

void EmitterSample::update() {
	isEmit = false;
	BaseEmitter::update();
	//if (std::fmod(timer, 1.0f) < WorldClock::DeltaSeconds()) {
	if (true) {
		isEmit = true;
	}
}

void EmitterSample::restart() {
	timer = 0;
}

void EmitterSample::on_emit(BaseParticle* const particle) {
	particle->get_transform().set_translate(
		this->world_position() + Vector3{ EmitterRandomSample::ufd1010(EmitterRandomSample::mt), 0, EmitterRandomSample::ufd1010(EmitterRandomSample::mt) }
		);
}
