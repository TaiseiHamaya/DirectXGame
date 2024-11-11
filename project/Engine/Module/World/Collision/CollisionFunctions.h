#pragma once

#include "Engine/Module/World/Collision/Collider/SphereCollider.h"

bool Collision(const SphereCollider* lhs, const SphereCollider* rhs) {
	Vector3 distance = lhs->world_position() - rhs->world_position();
	float range = lhs->get_radius() + rhs->get_radius();
	return distance.length() <= range;
}