#include "Hierarchy.h"

#include "Engine/Math/Matrix4x4.h"
#include "Engine/Module/WorldInstance/WorldInstance.h"

void Hierarchy::initialize(const Matrix4x4& worldMatrix) noexcept {
	currentMatrix = &worldMatrix;
}

void Hierarchy::set_parent(const WorldInstance& hierarchy) noexcept {
	parent = &hierarchy;
}

void Hierarchy::reset_parent() noexcept {
	parent = nullptr;
}

const WorldInstance& Hierarchy::get_parent() const noexcept {
	return *parent;
}

const WorldInstance* Hierarchy::get_parent_address() const noexcept {
	return parent;
}

bool Hierarchy::has_parent() const noexcept {
	return parent;
}

const Matrix4x4& Hierarchy::parent_matrix() const {
	return parent->world_matrix();
}

const Matrix4x4& Hierarchy::parent_matrix_safe() const noexcept {
	if (has_parent()) {
		return parent_matrix();
	}
	return CMatrix4x4::IDENTITY;
}
