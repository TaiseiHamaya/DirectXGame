#include "BaseCollider.h"

#include "Library/Math/Hierarchy.h"

BaseCollider::BaseCollider() {
	hierarchy.initialize(world_affine());
	groupName = nullptr;
}

void BaseCollider::update() {
	update_affine();
}

const std::string& BaseCollider::group() const noexcept {
	if (groupName) {
		return *groupName;
	}
	else {
		static std::string ColliderZeroString{};
		return ColliderZeroString;
	}
}

void BaseCollider::set_group_name(const std::string& name) {
	groupName = &name;
}
