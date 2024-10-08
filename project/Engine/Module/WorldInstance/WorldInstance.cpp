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

Matrix4x4 WorldInstance::create_world_matrix() const {
	Matrix4x4 result = transform.get_matrix();
	if (hierarchy.has_parent()) {
		result *= hierarchy.parent_matrix();
	}
	return result;
}

void WorldInstance::set_active(bool isActive_) {
	isActive = isActive_;
}

const Hierarchy& WorldInstance::get_hierarchy() const {
	return hierarchy;
}

Hierarchy& WorldInstance::get_hierarchy() {
	return hierarchy;
}

const Transform3D& WorldInstance::get_transform() const {
	return transform;
}

Transform3D& WorldInstance::get_transform() {
	return transform;
}

const Matrix4x4& WorldInstance::world_matrix() const {
	return worldMatrix;
}

Vector3 WorldInstance::world_position() const {
	return Transform3D::ExtractPosition(worldMatrix);
}

void WorldInstance::set_parent(const Hierarchy& hierarchy_) {
	hierarchy.set_parent(hierarchy_);
}

#ifdef _DEBUG
#include <imgui.h>
void WorldInstance::debug_gui() {
	ImGui::Checkbox("Active", &isActive);
	transform.debug_gui();
}
#endif // _DEBUG
