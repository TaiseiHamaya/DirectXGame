#include "ParticleFactorySample.h"

#include "ParticleSample.h"

#include <random>

#include <Engine/Utility/Tools/SmartPointer.h>
#include "Engine/Module/World/Collision/CollisionManager.h"

static std::random_device device{};
static std::mt19937 mt{ device() };
static std::uniform_real_distribution<float> ufd10{ 0,10 };
static std::uniform_real_distribution<float> ufd1010{ -10,10 };
static std::uniform_real_distribution<float> ufd1{ 0,1 };

std::unique_ptr<BaseParticle> ParticleFactorySample::factory() const {
	auto result = eps::CreateUnique<ParticleSample>(Vector3{ 0, ufd10(mt),0 }, camera);
	collisionManager->register_collider("Particle", result->get_collider());
	return std::move(result);
}
