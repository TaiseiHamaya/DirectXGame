#include "WorldInstance.h"

void WorldInstance::initialize() {
	hierarchy.initialize(worldMatrix);
}

void WorldInstance::update_matrix() {
	if (!isActive) {
		return;
	}
	worldMatrix = create_world_matrix();
}

void WorldInstance::look_at(const WorldInstance& rhs, const Vector3& upward) noexcept {
	look_at(rhs.world_position(), upward);
}

// 既知の不具合 : 特定環境でlook_atが正しくならない場合がある
void WorldInstance::look_at(const Vector3& point, const Vector3& upward) noexcept {
	Vector3 localForward;
	Vector3 localUpward;
	if (hierarchy.has_parent()) {
		Matrix4x4 parentInversedWorldMatrix = hierarchy.parent_matrix().inverse();
		Vector3 rhsObjectCoordinatePosition = Transform3D::Homogeneous(point, parentInversedWorldMatrix);
		localUpward = Transform3D::HomogeneousVector(upward, parentInversedWorldMatrix);
		localForward = (rhsObjectCoordinatePosition - transform.get_translate()).normalize_safe();
	}
	else {
		localUpward = upward;
		localForward = (point - transform.get_translate()).normalize_safe();
	}
	transform.set_quaternion(Quaternion::LookForward(localForward, localUpward));
}

Matrix4x4 WorldInstance::create_world_matrix() const {
	Matrix4x4 result = transform.create_matrix();
	if (hierarchy.has_parent()) {
		result *= hierarchy.parent_matrix();
	}
	return result;
}

#ifdef _DEBUG
#include <imgui.h>
void WorldInstance::debug_gui() {
	ImGui::Checkbox("Active", &isActive);
	transform.debug_gui();
}
#endif // _DEBUG
