#include "SphereCollider.h"

void SphereCollider::initialize() {
#ifdef _DEBUG
	colliderDrawer = std::make_unique<GameObject>();
	colliderDrawer->reset_object("SphereCollider.obj");
	colliderDrawer->set_parent(*hierarchy);
#endif // _DEBUG

	set_radius(1.0f);
}

void SphereCollider::set_radius(float radius_) {
	radius = radius_;

#ifdef _DEBUG
	colliderDrawer->get_transform().set_scale(Vector3{radius,radius,radius});
#endif // _DEBUG
}

float SphereCollider::get_radius() {
	return radius;
}
