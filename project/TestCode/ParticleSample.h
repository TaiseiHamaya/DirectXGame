#pragma once

#include "Engine/Module/ParticleSystem/Particle/Movements/BaseParticleMovements.h"

class ParticleSample : public BaseParticleMovements {
public: // Member function
	std::unique_ptr<BaseParticleMovements> clone() override;
	void initialize(Particle* const particle) override;
	void move(Particle* const particle) override;

	void set_camera(WorldInstance* camera_) { camera = camera_; };

private:
	float timer;
	float lifeTime = 100;
	Vector3 velocity;

	WorldInstance* camera;
};
