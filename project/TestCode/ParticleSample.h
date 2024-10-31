#pragma once

#include "Engine/Module/ParticleSystem/Particle/BaseParticle.h"
#include "Engine/Module/World/Collision/Collider/SphereCollider.h"

class ParticleSample : public BaseParticle {
public:
	ParticleSample(
		const Vector3& velocity_,
		const WorldInstance* camera_
	);

public:
	void initialize() override;
	void update() override;

private:
	void on_collision(const BaseCollider* const other);

public:
	const std::shared_ptr<SphereCollider> get_collider() const { return collider; };

private:
	float timer = 0;
	float lifeTime = 100;
	Vector3 velocity;
	std::shared_ptr<SphereCollider> collider;

	const WorldInstance* camera;
};
