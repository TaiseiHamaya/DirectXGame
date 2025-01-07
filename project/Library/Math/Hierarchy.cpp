#include "Hierarchy.h"

#include "Library/Math/Affine.h"
#include "Engine/Module/World/WorldInstance/WorldInstance.h"

void Hierarchy::initialize(const Affine& worldMatrix) noexcept {
	currentMatrix = &worldMatrix;
}

void Hierarchy::set_parent(const WorldInstance& hierarchy) noexcept {
	parent = &hierarchy;
}

void Hierarchy::reset_parent() noexcept {
	parent = nullptr;
}

const Reference<const WorldInstance>& Hierarchy::get_parent() const noexcept {
	return parent;
}

//const WorldInstance* Hierarchy::get_parent_address() const noexcept {
//	return parent;
//}

bool Hierarchy::has_parent() const noexcept {
	return parent;
}

const Affine& Hierarchy::parent_affine() const {
	return parent->world_affine();
}

const Affine& Hierarchy::parent_matrix_safe() const noexcept {
	if (has_parent()) {
		return parent_affine();
	}
	return CAffine::IDENTITY;
}
