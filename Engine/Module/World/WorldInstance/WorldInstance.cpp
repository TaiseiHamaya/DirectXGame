#include "WorldInstance.h"

#include "../WorldManager.h"

void WorldInstance::update_affine() {
	if (!isActive) {
		return;
	}
	affine = create_world_affine();
}

Affine WorldInstance::create_world_affine() const {
	Affine result = Affine::FromTransform3D(transform);
	if (hierarchy.has_parent()) {
		result *= hierarchy.parent_affine();
	}
	return result;
}

void WorldInstance::look_at(Reference<const WorldInstance> rhs, r32 angle, const Vector3& upward) noexcept {
	if (!rhs) {
		return;
	}
	look_at(rhs->world_position(), angle, upward);
}

// 既知の不具合 : 特定環境でlook_atが正しくならない場合がある
void WorldInstance::look_at(const Vector3& point, r32 angle, const Vector3& upward) noexcept {
	Vector3 localForward;
	Vector3 localUpward;
	if (hierarchy.has_parent()) {
		Affine parentInversedWorldAffine = hierarchy.parent_affine().inverse_fast();
		Vector3 rhsObjectCoordinatePosition = point * parentInversedWorldAffine;
		localUpward = upward * parentInversedWorldAffine.get_basis();
		localForward = (rhsObjectCoordinatePosition - transform.get_translate()).normalize_safe();
	}
	else {
		localUpward = upward;
		localForward = (point - transform.get_translate()).normalize_safe();
	}
	transform.set_quaternion(Quaternion::AngleAxis(localForward, angle) * Quaternion::LookForward(localForward, localUpward));
}

void WorldInstance::look_at_axis(Reference<const WorldInstance> target, const Vector3& axis, r32 angle) noexcept {
	if (!target) {
		return;
	}
	look_at_axis(target->world_position(), axis, angle);
}

void WorldInstance::look_at_axis(const Vector3& point, const Vector3& axis, r32 angle) noexcept {
	Vector3 toTarget = world_position() - point;
	Vector3 lookPosition = point + Vector3::Projection(toTarget, axis);
	look_at(lookPosition, angle, axis);
}

void WorldInstance::reparent(Reference<const WorldInstance> instance, bool isKeepPose) {
	const Affine& worldAffine = this->world_affine();
	if (isKeepPose) {
		if (instance) {
			const Affine parentAffineInv = instance->world_affine().inverse();
			const Affine local = worldAffine * parentAffineInv;
			const Basis& basis = local.get_basis();
			transform.set_scale(basis.to_scale());
			transform.set_quaternion(basis.to_quaternion());
			transform.set_translate(local.get_origin());
		}
		else {
			const Basis& basis = worldAffine.get_basis();
			transform.set_scale(basis.to_scale());
			transform.set_quaternion(basis.to_quaternion());
			transform.set_translate(worldAffine.get_origin());
		}
	}
	hierarchy.set_parent(*instance.ptr());
	if (instance) {
		hierarchyDepth = instance->depth() + 1;
	}
	else {
		hierarchyDepth = 0;
	}
	if (worldManager) {
		worldManager->reset_depth(this, hierarchyDepth);
	}
}

void WorldInstance::set_world_manager(Reference<WorldManager> worldManager_) {
	worldManager = worldManager_;
}
