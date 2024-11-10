#include "BaseCollider.h"

#include "Library/Math/Hierarchy.h"

BaseCollider::BaseCollider() {
	hierarchy.initialize(world_matrix());
	groupName = nullptr;

#ifdef _DEBUG
	colliderDrawer = std::make_unique<MeshInstance>();
	colliderDrawer->set_parent(*this);
#endif // _DEBUG
}

void BaseCollider::update() {
	update_matrix();
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

void BaseCollider::set_on_collision(std::function<void(const BaseCollider* const)> function) {
	onCollisionFunc = function;
}

void BaseCollider::set_on_collision_enter(std::function<void(const BaseCollider* const)> function) {
	onCollisionEnterFunc = function;
}

void BaseCollider::set_on_collision_exit(std::function<void(const BaseCollider* const)> function) {
	onCollisionExitFunc = function;
}

void BaseCollider::set_group_name(const std::string& name) {
	groupName = &name;
}

#ifdef _DEBUG
MeshInstance& BaseCollider::get_collider_drawer() const {
	return *colliderDrawer;
}
#endif // _DEBUG
