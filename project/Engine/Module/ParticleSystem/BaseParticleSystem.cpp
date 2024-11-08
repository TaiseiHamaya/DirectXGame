#include "BaseParticleSystem.h"

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
	for (std::unique_ptr<BaseParticle>& particle : particles) {
		if (particle->is_active()) {
			particle->update();
		}
	}
	// 削除
	particles.remove_if(
		[&](std::unique_ptr<BaseParticle>& particle) {
		if (particle->is_destroy()) {
			return true;
		}
		return false;
	});
	// 生成
	if (emitter->is_active() && emitter->is_emit() && !emitter->is_end()) {
		emit();
	}
}

void BaseParticleSystem::begin_rendering() {
	emitter->update_matrix();
	for (uint32_t index = 0; std::unique_ptr<BaseParticle>& particle : particles) {
		particle->update_matrix();
		particleBuffer.get_array()[index] = {
			particle->world_matrix(),
			particle->create_uv_matrix(),
			particle->get_color()
		};
		++index;
	}
}

void BaseParticleSystem::emit() {
	uint32_t numEmits = emitter->num_emits();
	for (uint32_t i = 0; i < numEmits; ++i) {
		size_t numParticle = particles.size();
		if (numParticle < numMaxParticle) {
			emit_once();
		}
	}
}

void BaseParticleSystem::emit_once() {
	auto& newParticle = particles.emplace_back(
		factory ? factory->factory() : nullptr
	);
	if (newParticle) {
		newParticle->initialize();
		if (emitter) {
			emitter->on_emit(newParticle.get());
		}
	}
}

bool BaseParticleSystem::is_end_all() const {
	return emitter->is_end() && particles.empty();
}

void BaseParticleSystem::set_emitter(std::unique_ptr<BaseEmitter>&& emitter_) {
	emitter = std::move(emitter_);
	emitter->initialize();
}

void BaseParticleSystem::create_buffers() {
	particleBuffer.initialize(numMaxParticle);
}

#ifdef _DEBUG

#include <imgui.h>
void BaseParticleSystem::debug_gui() {
	if (ImGui::CollapsingHeader("Emitter")) {
		emitter->debug_gui();
	}
	if (ImGui::CollapsingHeader("Particles")) {
		ImGui::Text("Now/Max : %d/%d", particles.size(), numMaxParticle);
	}
}
#endif // _DEBUG
