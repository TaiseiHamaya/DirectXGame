#pragma once

#include "Particle/Particle.h"
#include "Emitter/BaseEmitter.h"
#include "Engine/DirectX/DirectXResourceObject/StructuredBuffer/StructuredBuffer.h"
#include "ParticleBufferValue.h"

#include <memory>
#include <list>

class BaseParticleMovements;

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

public:
	const BaseEmitter& get_emitter() const { return *emitter; };
	const std::list<Particle>& get_particles() const { return particles; };

	void set_emitter(std::unique_ptr<BaseEmitter>&& emitter_);
	void set_particle_movements(std::unique_ptr<BaseParticleMovements>&& particleMovements_);

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
	std::list<Particle> particles;

	std::unique_ptr<BaseParticleMovements> particleMovements;

protected:
	StructuredBuffer<ParticleBuffer> particleBuffer;
};
