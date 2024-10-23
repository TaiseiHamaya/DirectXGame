#include "ParticleSample.h"

#include "Engine/Utility/SmartPointer.h"
#include "Engine/Application/WorldClock/WorldClock.h"

#include <random>

static std::random_device device{};
static std::mt19937 mt{ device() };
static std::uniform_real_distribution<float> ufd10{ 0,10 };
static std::uniform_real_distribution<float> ufd1010{ -10,10 };
static std::uniform_real_distribution<float> ufd1{ 0,1 };

std::unique_ptr<BaseParticleMovements> ParticleSample::clone() {
	auto result = eps::CreateUnique<ParticleSample>();
	result->velocity = { 0, ufd10(mt),0 };
	return std::move(result);
}

void ParticleSample::initialize(Particle* const particle) {
	// do nothing
}

void ParticleSample::move(Particle* const particle) {
	timer += WorldClock::DeltaSeconds();
	Vector3 gravity = Vector3{ 0,-9.8f,0 };
	velocity += gravity * WorldClock::DeltaSeconds();
	particle->get_transform().plus_translate(velocity * WorldClock::DeltaSeconds());
	float base = std::lerp(1.0f, 0.0f, std::clamp(timer / lifeTime, 0.0f, 1.0f));
	particle->set_color({ base,base,base, base});
	if (particle->get_transform().get_translate().y <= 0) {
		particle->set_destroy();
	}
}
