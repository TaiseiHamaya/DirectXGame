#include "Particle.h"

#include "Movements/BaseParticleMovements.h"

Particle::Particle(
	uint32_t useIndex_,
	ParticleBuffer& particleBuffer_,
	std::unique_ptr<BaseParticleMovements>&& movements_
) :
	particleBuffer(particleBuffer_),
	useIndex(useIndex_),
	movements(std::move(movements_)) {
	initialize();
}

void Particle::update() {
	if (movements) {
		movements->move(this);
	}
}

void Particle::begin_rendering() {
	WorldInstance::update_matrix();
	particleBuffer.world = world_matrix();
}
