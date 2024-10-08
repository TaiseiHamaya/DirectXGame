#pragma once

#include "Particle/BaseParticle.h"
#include "Emitter/BaseEmitter.h"
#include "Engine/DirectX/DirectXResourceObject/StructuredBuffer/StructuredBuffer.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/Texture.h"
#include "Engine/Module/Color/Color.h"

#include <memory>
#include <list>

class ParticleSystem {
private:
	struct Transform {
		Matrix4x4 world;
	};

	struct Material {
		Color color;
	};

public:
	ParticleSystem() = default;
	~ParticleSystem() = default;

	ParticleSystem(const ParticleSystem&) = delete;
	ParticleSystem& operator=(const ParticleSystem&) = delete;
	ParticleSystem(ParticleSystem&&) = default;
	ParticleSystem& operator=(ParticleSystem&&) = default;

public:
	void initialize(uint32_t numMaxParticle_);
	void finalize();

private:
	void create_buffer();

private:
	uint32_t numMaxParticle;
	std::unique_ptr<BaseEmitter> emitter;
	std::list<BaseParticle> particle;

	std::weak_ptr<Texture> texutre;
	StructuredBuffer<Transform> transformBuffer;
	StructuredBuffer<Material> materialBuffer;
};
