#pragma once

#include <Engine/Module/ParticleSystem/Particle/ParticleFactroy.h>

class WorldInstance;
class CollisionManager;

class ParticleFactorySample final : public ParticleFactory {
public:
	ParticleFactorySample(
		const WorldInstance* camera_,
		CollisionManager* collisionManager_
	) : camera(camera_) ,collisionManager(collisionManager_) {};

public:
	std::unique_ptr<BaseParticle> factory()const;

private:
	const WorldInstance* camera;
	CollisionManager* collisionManager;
};
