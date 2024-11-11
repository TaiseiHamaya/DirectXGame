#include "EnemyManager.h"

#include <Engine/Utility/Tools/SmartPointer.h>

#include <Engine/Module/World/Collision/CollisionManager.h>

#include "Game/GameScene/ScoreManager/ScoreManager.h"

void EnemyManager::initialize(CollisionManager* collisionManager_, ScoreManager* scoreManager_) {
	collisionManager = collisionManager_;
	scoreManager = scoreManager_;

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

	enemies.remove_if(
		[](const std::unique_ptr<BaseEnemy>& enemy) { return enemy->is_dead(); }
	);
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
	if (enemy->is_dead()) {
		scoreManager->register_enemy(enemy);
	}
}

void EnemyManager::create() {
	auto& newEnemy = enemies.emplace_back(
		eps::CreateUnique<BaseEnemy>()
	);
	newEnemy->initialize("enemy.obj", 0.2f, 5, 100, 30);
	newEnemy->get_transform().set_translate(Vector3{ 0.0f, 0.8f, 3.0f });

	collisionManager->register_collider("Enemy", newEnemy->get_collider());
	reverseEnemies.emplace(newEnemy->get_collider().get(), newEnemy.get());
}
