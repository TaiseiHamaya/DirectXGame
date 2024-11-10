#pragma once

#include <Engine/Module/ParticleSystem/Particle/ParticleFactroy.h>

class WorldInstance;

class ParticleFactorySample final : public ParticleFactory {
public:
	ParticleFactorySample(WorldInstance* camera_) : camera(camera_) {};

public:
	std::unique_ptr<BaseParticle> factory()const;

private:
	WorldInstance* camera;
};
