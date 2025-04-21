#pragma once

#include "Engine/Module/World/Collision/Collider/AABBCollider.h"
#include "Engine/Module/World/Collision/Collider/SphereCollider.h"

bool Collision(const SphereCollider& lhs, const SphereCollider& rhs) {
	Vector3 distance = lhs.world_position() - rhs.world_position();
	float range = lhs.get_radius() + rhs.get_radius();
	return distance.length() <= range;
}

bool Collision(const AABBCollider& lhs, const AABBCollider& rhs) {
	const Vector3& lhsMin = lhs.min_vertex();
	const Vector3& lhsMax = lhs.max_vertex();
	const Vector3& rhsMin = rhs.min_vertex();
	const Vector3& rhsMax = rhs.max_vertex();

	if (
		lhsMin.x <= rhsMax.x && lhsMax.x >= rhsMin.x &&
		lhsMin.y <= rhsMax.y && lhsMax.y >= rhsMin.y &&
		lhsMin.z <= rhsMax.z && lhsMax.z >= rhsMin.z
		) {
		return true;
	}
	return false;
}

bool Collision(const AABBCollider& aabb, const SphereCollider& sphere) {
	const Vector3& sphereWorld = sphere.world_position();
	const Vector3& aabbWorld = aabb.world_position();
	Vector3 closest = Vector3::Clamp(sphereWorld, aabb.min_vertex() + aabbWorld, aabb.max_vertex() + aabbWorld);
	if ((closest - sphereWorld).length() <= sphere.get_radius()) {
		return true;
	}
	return false;
}

bool Collision(const SphereCollider& sphere, const AABBCollider& aabb) {
	return Collision(aabb, sphere);
}
