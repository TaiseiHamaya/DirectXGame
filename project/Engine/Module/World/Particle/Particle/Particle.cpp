#include "Particle.h"

#include "Engine/Runtime/WorldClock/WorldClock.h"

Particle::Particle(const Vector3& translate, float lifetime_, const Vector3& velocity_, const Vector3& acceleration_, const Color4& startColor_, const Color4& endColor_, const Vector3& startSize_, const Vector3& endSize_) :
	lifetime(lifetime_),
	velocity(velocity_), acceleration(acceleration_),
	startColor(startColor_), endColor(endColor_),
	startSize(startSize_), endSize(endSize_) {
	isDestroy = false;
	timer = 0;
	transform.set_translate(translate);
}

void Particle::update() {
	timer += WorldClock::DeltaSeconds();

	float parametric = std::min(1.0f, timer / lifetime);

	velocity += acceleration * WorldClock::DeltaSeconds();
	transform.plus_translate(velocity * WorldClock::DeltaSeconds());

	color = Color4::Lerp(startColor, endColor, parametric);
	transform.set_scale(Vector3::Lerp(startSize, endSize, parametric));

	if (timer >= lifetime) {
		isDestroy = true;
	}
}

const Matrix4x4 Particle::create_uv_matrix() const {
	return uvTransform.get_matrix4x4_padding();
}
