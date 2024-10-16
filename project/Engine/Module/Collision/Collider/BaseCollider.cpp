#include "BaseCollider.h"

#include "Engine/Module/Hierarchy/Hierarchy.h"

BaseCollider::BaseCollider() {
	hierarchy.initialize(world_matrix());
	groupName = nullptr;

#ifdef _DEBUG
	colliderDrawer = std::make_unique<GameObject>();
	colliderDrawer->set_parent(*this);
#endif // _DEBUG
}

void BaseCollider::begin() {
	if (!isActive) {
		collisionMap.clear();
		return;
	}
	for (auto itr = collisionMap.begin(); itr != collisionMap.end();){
		if (itr->second.none()) {
			itr = collisionMap.erase(itr);
		}
		else {
			itr->second <<= 1;
			++itr;
		}
	}
}

void BaseCollider::update() {
	update_matrix();
}

void BaseCollider::collision(const BaseCollider* const collider, bool result) {
	if (!isActive) {
		return;
	}
	if (!collisionMap.contains(collider)) {
		collisionMap.emplace(collider, "00");
	}

	auto& collisionObject = collisionMap.at(collider);

	collisionObject.set(0, result);
	unsigned long state = collisionObject.to_ulong();
	// このフレーム衝突しているか
	switch (state) {
	case 0b00:
		// 前フレームも衝突していない
		break;
	case 0b01:
		// 今はじめて衝突
			// Enter判定
		if (onCollisionEnterFunc) {
			onCollisionEnterFunc(collider);
		}
		else if (onCollisionFunc) {
			onCollisionFunc(collider);
		}
		break;
	case 0b10:
		// Exit
		if (onCollisionExitFunc) {
			onCollisionExitFunc(collider);
		}
		else if (onCollisionFunc) {
			onCollisionFunc(collider);
		}
		break;
	case 0b11:
		// 連続衝突
		if (onCollisionFunc) {
			onCollisionFunc(collider);
		}
		break;
	default:
		break;
	}
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
GameObject& BaseCollider::get_collider_drawer() const {
	return *colliderDrawer;
}
#endif // _DEBUG
