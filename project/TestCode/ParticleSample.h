#pragma once

#include "Engine/Module/ParticleSystem/Particle/BaseParticle.h"

class ParticleSample : public BaseParticle {
public:
	ParticleSample(
		const Vector3& velocity_,
		WorldInstance* camera_
	);

public:
	void initialize() override;
	void update() override;

private:
	float timer = 0;
	float lifeTime = 100;
	Vector3 velocity;

	WorldInstance* camera;
};
