#pragma once

#include "Engine/Module/ParticleSystem/Particle/Movements/BaseParticleMovements.h"

class ParticleSample : public BaseParticleMovements {
public: // Member function
	std::unique_ptr<BaseParticleMovements> clone();
	void move(Particle* const particle);

private:
	float timer;
	float lifeTime = 1;
	Vector3 translate;
};
