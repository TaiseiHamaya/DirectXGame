#include "ParticleSample.h"

#include "Engine/Utility/SmartPointer.h"
#include "Engine/Application/WorldClock/WorldClock.h"

#include <random>

static std::random_device device{};
static std::mt19937 mt{ device() };
static std::uniform_real_distribution<float> ufd10{ -10,10 };
static std::uniform_real_distribution<float> ufd1{ 0,1 };

std::unique_ptr<BaseParticleMovements> ParticleSample::clone() {
	auto result = eps::CreateUnique<ParticleSample>();
	result->translate = { ufd10(mt),0,ufd10(mt) };
	return std::move(result);
}

void ParticleSample::move(Particle* const particle) {
	timer += WorldClock::DeltaSeconds();
	particle->get_transform().set_translate(translate);
	float base = std::lerp(1.0f, 0.0f, std::clamp(timer / lifeTime, 0.0f, 1.0f));
	particle->set_color({ base,base,base, base});
	if (timer >= lifeTime) {
		particle->set_destroy();
	}
}
