#include "EnemyManager.h"

#include <Engine/Utility/Tools/SmartPointer.h>

#include <Engine/Module/World/Collision/CollisionManager.h>

void EnemyManager::initialize(CollisionManager* collisionManager_) {
	collisionManager = collisionManager_;

	create();
}

void EnemyManager::begin() {
	for (std::unique_ptr<BaseEnemy>& enemy : enemies) {
		enemy->begin();
	}
}

void EnemyManager::update() {
	for (std::unique_ptr<BaseEnemy>& enemy : enemies) {
		enemy->update();
	}
}

void EnemyManager::begin_rendering() noexcept {
	for (std::unique_ptr<BaseEnemy>& enemy : enemies) {
		enemy->begin_rendering();
	}
}

void EnemyManager::late_update() {
	for (std::unique_ptr<BaseEnemy>& enemy : enemies) {
		enemy->late_update();
	}
}

void EnemyManager::draw() const {
	for (const std::unique_ptr<BaseEnemy>& enemy : enemies) {
		enemy->draw();
	}
}

void EnemyManager::callback_collider(BaseCollider* const collider) {
	if (!reverseEnemies.contains(collider)) {
		return;
	}
	BaseEnemy* enemy = reverseEnemies.at(collider);
	enemy->hit();
}

void EnemyManager::create() {
	auto& newEnemy = enemies.emplace_back(
		eps::CreateUnique<BaseEnemy>()
	);
	newEnemy->initialize();
	newEnemy->get_transform().set_translate(Vector3{ 0.0f, 0.8f, 3.0f });

	collisionManager->register_collider("Enemy", newEnemy->get_collider());
	reverseEnemies.emplace(newEnemy->get_collider().get(), newEnemy.get());
}
