#include "ParticleSample.h"

#include "Engine/Runtime/WorldClock/WorldClock.h"

#include <algorithm>

ParticleSample::ParticleSample(const Vector3& velocity_, WorldInstance* camera_) :
	velocity(velocity_), camera(camera_), timer(0) {

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
