#include "SphereCollider.h"

void SphereCollider::initialize() {
	set_radius(1.0f);
}

void SphereCollider::set_radius(float radius_) {
	radius = radius_;
}

float SphereCollider::get_radius() const {
	return radius;
}
