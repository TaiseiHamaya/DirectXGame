#include "ParticleSystem.h"

void ParticleSystem::initialize(uint32_t numMaxParticle_) {
	numMaxParticle = numMaxParticle_;
}

void ParticleSystem::finalize() {
	transformBuffer.finalize();
	materialBuffer.finalize();
}

void ParticleSystem::create_buffer() {
	transformBuffer.initialize(numMaxParticle);
	materialBuffer.initialize(numMaxParticle);
}
