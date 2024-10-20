#pragma once

#include "Engine/Module/ParticleSystem/ParticleBufferValue.h"
#include "Engine/Module/WorldInstance/WorldInstance.h"

#include <memory>

class BaseParticleMovements;

class Particle final : public WorldInstance {
public: // Constructor/Destructor
	Particle(
		uint32_t useIndex, 
		ParticleBuffer& particleBuffer_,
		std::unique_ptr<BaseParticleMovements>&& movements_
	);
	~Particle() = default;

	Particle(const Particle&) = delete;
	Particle& operator=(const Particle&) = delete;
	Particle(Particle&&) = delete;
	Particle& operator=(Particle&&) = delete;

public: // Member function
	void update();
	void begin_rendering();

public: // Getter/Setter
	void set_color(const Color& color) { particleBuffer.color = color; };
	void set_destroy() { isDestroy = true; };

	bool is_destroy() const { return isDestroy; };
	uint32_t used_index() const { return useIndex; };

private: // Member variable
	uint32_t useIndex;

	bool isDestroy = false;
	ParticleBuffer& particleBuffer;

	std::unique_ptr<BaseParticleMovements> movements;
};
