#include "SphereCollider.h"

SphereCollider::SphereCollider(float radius_) :
	radius(radius_) {

#ifdef _DEBUG
	debugMatrix = Transform3D::MakeScaleMatrix({ radius, radius , radius });
#endif // _DEBUG
}

void SphereCollider::set_radius(float radius_) {
	radius = radius_;

#ifdef _DEBUG
	debugMatrix = Transform3D::MakeScaleMatrix({ radius, radius , radius });
#endif // _DEBUG
}

float SphereCollider::get_radius() const {
	return radius;
}
