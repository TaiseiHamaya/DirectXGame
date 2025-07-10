#include "Hierarchy.h"

#include "Affine.h"

#include "Engine/Module/World/WorldInstance/WorldInstance.h"

Hierarchy::Hierarchy(Reference<const Affine> selfAffine_) {
	selfAffine = selfAffine_;
	parent = nullptr;
}

void Hierarchy::set_parent(const WorldInstance& instance) noexcept {
	parent = instance;
}

void Hierarchy::reset_parent() noexcept {
	parent = nullptr;
}

const Reference<const WorldInstance>& Hierarchy::get_parent() const noexcept {
	return parent;
}

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
