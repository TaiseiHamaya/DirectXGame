#pragma once

#include "Particle/BaseParticle.h"
#include "Particle/ParticleFactroy.h"
#include "Emitter/BaseEmitter.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/StructuredBuffer/StructuredBuffer.h"
#include "ParticleBufferValue.h"

#include <memory>
#include <list>

class BaseParticleSystem {
public:
	BaseParticleSystem();
	virtual ~BaseParticleSystem();

	BaseParticleSystem(const BaseParticleSystem&) = delete;
	BaseParticleSystem& operator=(const BaseParticleSystem&) = delete;
	BaseParticleSystem(BaseParticleSystem&&) = default;
	BaseParticleSystem& operator=(BaseParticleSystem&&) = default;

public:
	void initialize(uint32_t numMaxParticle_);
	void finalize();

	void update();

	void begin_rendering();

	virtual void draw() const = 0;

	void emit();
	void emit_once();

public:
	bool is_end_all() const;
	const BaseEmitter& get_emitter() const { return *emitter; };
	const std::list<std::unique_ptr<BaseParticle>>& get_particles() const { return particles; };

	void set_emitter(std::unique_ptr<BaseEmitter>&& emitter_);
	void set_factory(std::unique_ptr<ParticleFactory>&& factory_) { factory = std::move(factory_); };

private:
	void create_buffers();

#ifdef _DEBUG
public:
	void debug_gui();
#endif // _DEBUG

protected:
	uint32_t numMaxParticle{ 0 };
	
private:
	std::unique_ptr<BaseEmitter> emitter;
	std::list<std::unique_ptr<BaseParticle>> particles;
	std::unique_ptr<ParticleFactory> factory;

protected:
	StructuredBuffer<ParticleBuffer> particleBuffer;
};
