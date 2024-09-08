#include "BaseCollider.h"

#include "Engine/Game/Hierarchy/Hierarchy.h"

BaseCollider::BaseCollider() {
	hierarchy = std::make_unique<Hierarchy>();
	hierarchy->initialize(worldMatrix);
	transform = std::make_unique<Transform3D>();

#ifdef _DEBUG
	colliderDrawer = std::make_unique<GameObject>();
	colliderDrawer->set_parent(*hierarchy);
#endif // _DEBUG
}

void BaseCollider::begin() {
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
	worldMatrix = transform->get_matrix();
	if (hierarchy->has_parent()) {
		worldMatrix *= hierarchy->parent_matrix();
	}
}

void BaseCollider::collision(const BaseCollider* const collider, bool result) {
	if (!collisionMap.contains(collider)) {
		collisionMap.emplace(collider, "00");
	}

	auto& collisionObject = collisionMap.at(collider);

	collisionObject.set(0, result);
	unsigned int state = collisionObject.to_ulong();
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

Hierarchy& BaseCollider::get_hierarchy() const {
	return *hierarchy;
}

Transform3D& BaseCollider::get_transform() const {
	return *transform;
}

Vector3 BaseCollider::world_position() const {
	return Transform3D::ExtractPosition(worldMatrix);
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

#ifdef _DEBUG
GameObject& BaseCollider::get_collider_drawer() const {
	return *colliderDrawer;
}
#endif // _DEBUG
