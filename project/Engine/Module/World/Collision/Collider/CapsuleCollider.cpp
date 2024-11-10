#include "CapsuleCollider.h"

void CapsuleCollider::initialize() {
	radius = 1;
	length = 1;
	direction = CVector3::BASIS_Y;
}

void CapsuleCollider::initialize(float radius_, float length_, const Vector3& direction_) {
	radius = radius_;
	length = length_;
	direction = direction_;
}
