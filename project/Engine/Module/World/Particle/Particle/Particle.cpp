#include "Particle.h"

#include "Engine/Runtime/WorldClock/WorldClock.h"

#include <Library/Utility/Tools/RandomEngine.h>

Particle::Particle(
	const Vector3& translate,
	float lifetime_,
	const Vector3& velocity_, const Vector3& acceleration_,
	const Color4& startColor_, const Color4& endColor_,
	const Vector3& startSize_, const Vector3& endSize_,
	RotationType rotationType_, std::variant<Constant, std::monostate, Random> rotationData_
) : 
	WorldInstance(),
	lifetime(lifetime_),
	velocity(velocity_), acceleration(acceleration_),
	startColor(startColor_), endColor(endColor_),
	startSize(startSize_), endSize(endSize_),
	rotationType(rotationType_), rotationData(rotationData_) {
	isDestroy = false;
	timer = 0;
	transform.set_translate(translate);
	transform.set_scale(startSize_);
	switch (rotationType) {
	case Particle::RotationType::Constant:
	{
		const auto& data = std::get<Constant>(rotationData);
		transform.set_quaternion(data.value);
		break;
	}
	case Particle::RotationType::Velocity:
		transform.set_quaternion(Quaternion::LookForward(velocity.normalize_safe()));
		break;
	case Particle::RotationType::LookAt:
		look_at(*lookAtDefault);
		break;
	case Particle::RotationType::Random:
	{
		float cos = -2.0f * RandomEngine::Random01MOD() + 1.0f;
		float sin = std::sqrt(1.0f - cos * cos);
		float phi = PI2 * RandomEngine::Random01MOD();
		float radius = std::pow(RandomEngine::Random01MOD(), 1.0f / 3.0f);
		Vector3 axis = { sin * std::cos(phi), sin * std::sin(phi), cos };
		float angle = PI2 * RandomEngine::Random01MOD();
		transform.set_quaternion(Quaternion::AngleAxis(axis, angle));
		break;
	}
	default:
		break;
	}

	color = startColor_;
}

void Particle::update() {
	timer += WorldClock::DeltaSeconds();

	float parametric = std::min(1.0f, timer / lifetime);

	velocity += acceleration * WorldClock::DeltaSeconds();
	transform.plus_translate(velocity * WorldClock::DeltaSeconds());

	color = Color4::Lerp(startColor, endColor, parametric);

	transform.set_scale(Vector3::Lerp(startSize, endSize, parametric));

	switch (rotationType) {
	case Particle::RotationType::Constant:
	{
		const auto& data = std::get<Constant>(rotationData);
		transform.set_quaternion(data.value);
		break;
	}
	case Particle::RotationType::Velocity:
		transform.set_quaternion(Quaternion::LookForward(velocity.normalize_safe()));
		break;
	case Particle::RotationType::LookAt:
		look_at(*lookAtDefault);
		break;
	case Particle::RotationType::Random:
	{
		const auto& data = std::get<Random>(rotationData);
		Quaternion amount = Quaternion::AngleAxis(data.axis, data.angle * WorldClock::DeltaSeconds());
		const Quaternion& rotation = transform.get_quaternion();
		transform.set_quaternion(amount * rotation);
		break;
	}
	default:
		break;
	}

	if (timer >= lifetime) {
		isDestroy = true;
	}
}

const Matrix4x4 Particle::create_uv_matrix() const {
	return uvTransform.get_matrix4x4_padding();
}
