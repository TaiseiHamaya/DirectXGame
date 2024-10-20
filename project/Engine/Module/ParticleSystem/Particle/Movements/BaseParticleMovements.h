#pragma once

#include <memory>
#include "Engine/Module/ParticleSystem/Particle/Particle.h"

class BaseParticleMovements {
public: // Constructor/Destructor
	BaseParticleMovements() = default;
	virtual ~BaseParticleMovements() = default;

	BaseParticleMovements(const BaseParticleMovements&) = default;
	BaseParticleMovements& operator=(const BaseParticleMovements&) = default;

public: // Member function
	virtual std::unique_ptr<BaseParticleMovements> clone() = 0;
	virtual void move(Particle* const particle) = 0;
};
