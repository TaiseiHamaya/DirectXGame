#include "Hierarchy.h"

#include "Engine/Math/Matrix4x4.h"

void Hierarchy::initialize(const Matrix4x4& worldMatrix) noexcept {
	currentMatrix = &worldMatrix;
}

void Hierarchy::set_parent(const Hierarchy& hierarchy) noexcept {
	parent = &hierarchy;
}

const Hierarchy& Hierarchy::get_parent() const noexcept {
	return *parent;
}

bool Hierarchy::has_parent() const noexcept {
	return parent;
}

const Matrix4x4& Hierarchy::parent_matrix() const noexcept {
	if (parent) {
		return *parent->currentMatrix;
	}
	return CMatrix4x4::IDENTITY;
}
