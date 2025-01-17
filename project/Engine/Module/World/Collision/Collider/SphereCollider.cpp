#include "SphereCollider.h"

SphereCollider::SphereCollider(float radius_) :
	radius(radius_) {
}

void SphereCollider::set_radius(float radius_) {
	radius = radius_;
}

float SphereCollider::get_radius() const {
	return radius;
}
