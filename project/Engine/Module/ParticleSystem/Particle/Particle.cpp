#include "Particle.h"

#include "Movements/BaseParticleMovements.h"

Particle::Particle(std::unique_ptr<BaseParticleMovements>&& movements_) :
	movements(std::move(movements_)) {
}

void Particle::update() {
	if (movements) {
		movements->move(this);
	}
}
