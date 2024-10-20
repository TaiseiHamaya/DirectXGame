#pragma once

#include "Engine/Module/ParticleSystem/Particle/Movements/BaseParticleMovements.h"

class ParticleSample : public BaseParticleMovements {
public: // Member function
	std::unique_ptr<BaseParticleMovements> clone() override;
	void initialize(Particle* const particle) override;
	void move(Particle* const particle) override;

private:
	float timer;
	float lifeTime = 1;
	Vector3 velocity;
};
