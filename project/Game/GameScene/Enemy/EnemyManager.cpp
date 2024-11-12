#include "EnemyManager.h"

#include <Engine/Utility/Tools/SmartPointer.h>

#include <Engine/Module/World/Collision/CollisionManager.h>

#include "Game/GameScene/ScoreManager/ScoreManager.h"

void EnemyManager::initialize(CollisionManager* collisionManager_, ScoreManager* scoreManager_) {
	collisionManager = collisionManager_;
	scoreManager = scoreManager_;

	typeDatabase = eps::CreateUnique<EnemyTypeEditor>();
	typeDatabase->initialize();
	movementDatabase = eps::CreateUnique<EnemyMovementsEditor>();
	movementDatabase->initialize();
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
		[&](const std::unique_ptr<BaseEnemy>& enemy) {
		if (enemy->is_dead()) {
			destroy(enemy.get());
			return true;
		}
		return false;
	}
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

void EnemyManager::create(const Vector3& translate, const std::string& type, const std::string& movement) {
	auto& newEnemy = enemies.emplace_back(
		eps::CreateUnique<BaseEnemy>()
	);
	newEnemy->initialize(
		translate,
		typeDatabase->get_template(type),
		movementDatabase->get_template(movement)
	);

	collisionManager->register_collider("Enemy", newEnemy->get_collider());
	reverseEnemies.emplace(newEnemy->get_collider().get(), newEnemy.get());
}

void EnemyManager::destroy(BaseEnemy* enemy) {
	reverseEnemies.erase(enemy->get_collider().get());
}

void EnemyManager::debug_gui() {
	typeDatabase->debug_gui();
	movementDatabase->debug_gui();
}
