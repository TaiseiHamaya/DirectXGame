#include "BaseParticleSystem.h"

#include "Particle/Movements/BaseParticleMovements.h"

BaseParticleSystem::BaseParticleSystem() = default;

BaseParticleSystem::~BaseParticleSystem() = default;

void BaseParticleSystem::initialize(uint32_t numMaxParticle_) {
	numMaxParticle = numMaxParticle_;
	create_buffers();
}

void BaseParticleSystem::finalize() {
	particleBuffer.finalize();
}

void BaseParticleSystem::update() {
	if (!emitter) {
		return;
	}
	// エミッター更新
	if (emitter->is_active()) {
		emitter->update();
	}
	// パティクルの更新
	for (Particle& particle : particles) {
		if (particle.is_active()) {
			particle.update();
		}
	}
	// 削除
	particles.remove_if(
		[&](Particle& particle) {
		if (particle.is_destroy()) {
			return true;
		}
		return false;
	});
	// 生成
	if (emitter->is_active() && emitter->is_emit()) {
		uint32_t numEmits = emitter->num_emits();
		if (numEmits && !emitter->is_end()) {
			for (uint32_t i = 0; i < numEmits; ++i) {
				emit();
			}
		}
	}
}

void BaseParticleSystem::begin_rendering() {
	for (uint32_t index = 0; Particle& particle : particles) {
		particle.update_matrix();
		particleBuffer.get_array()[index] = {
			particle.world_matrix(),
			particle.get_color()
		};
		++index;
	}
}

void BaseParticleSystem::emit() {
	size_t next = particles.size();
	if (next < numMaxParticle) {
		auto& newParticle = particles.emplace_back(
			particleMovements->clone()
		);
		newParticle.initialize();
	}
}

void BaseParticleSystem::set_emitter(std::unique_ptr<BaseEmitter>&& emitter_) {
	emitter = std::move(emitter_);
	emitter->initialize();
}

void BaseParticleSystem::set_particle_movements(std::unique_ptr<BaseParticleMovements>&& particleMovements_) {
	particleMovements = std::move(particleMovements_);
}

void BaseParticleSystem::create_buffers() {
	particleBuffer.initialize(numMaxParticle);
}
