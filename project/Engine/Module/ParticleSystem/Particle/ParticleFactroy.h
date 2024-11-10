#pragma once

#include <memory>

class BaseParticle;

class ParticleFactory {
public:
	ParticleFactory() = default;
	virtual ~ParticleFactory() = default;

	ParticleFactory(const ParticleFactory&) = delete;
	ParticleFactory& operator=(const ParticleFactory&) =delete;

public:
	virtual std::unique_ptr<BaseParticle> factory() const = 0;
};

