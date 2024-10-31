#include "ParticleSample.h"

#include "Engine/Runtime/WorldClock/WorldClock.h"
#include <Engine/Utility/Tools/SmartPointer.h>

#include <algorithm>

ParticleSample::ParticleSample(const Vector3& velocity_, const WorldInstance* camera_) :
	velocity(velocity_), camera(camera_), timer(0) {
	collider = eps::CreateShared<SphereCollider>();
	collider->initialize();
	collider->set_parent(*this);
	collider->set_radius(0.0f);
	collider->set_on_collision(std::bind(&ParticleSample::on_collision, this, std::placeholders::_1));
}

void ParticleSample::initialize() {
	BaseParticle::initialize();
	if (camera) {
		Vector3 upward = CVector3::BASIS_Y * camera->get_transform().get_quaternion();
		look_at(*camera, upward);
	}
}

void ParticleSample::update() {
	timer += WorldClock::DeltaSeconds();
	Vector3 gravity = Vector3{ 0,-9.8f,0 };
	velocity += gravity * WorldClock::DeltaSeconds();
	transform.plus_translate(velocity * WorldClock::DeltaSeconds());
	float base = std::lerp(1.0f, 0.0f, std::clamp(timer / lifeTime, 0.0f, 1.0f));
	color = { base,base,base, base };
	if (camera) {
		Vector3 upward = CVector3::BASIS_Y * camera->get_transform().get_quaternion();
		look_at(*camera, upward);
	}
	if (transform.get_translate().y <= 0) {
		isDestroy = true;
	}
}

void ParticleSample::on_collision([[maybe_unused]] const BaseCollider* const other) {
	Vector3 wind = Vector3{ 5.0f, 0.0f,0.0f };
	velocity += wind * WorldClock::DeltaSeconds();
}
