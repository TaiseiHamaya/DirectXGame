#include "AABBCollider.h"

AABBCollider::AABBCollider(const Vector3& size_, const Vector3& offset_) {
	min = -Vector3::Multiply(size_, offset_);
	max = size_ + min;
}

const Vector3& AABBCollider::min_vertex() const {
	return min;
}

const Vector3& AABBCollider::max_vertex() const {
	return max;
}
