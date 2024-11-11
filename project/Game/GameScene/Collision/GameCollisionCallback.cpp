#include "GameCollisionCallback.h"

#include "Game/GameScene/Enemy/BaseEnemy.h"
#include "Game/GameScene/Enemy/EnemyManager.h"

GameCollisionCallback::GameCollisionCallback(EnemyManager* enemyManager_) :
	enemyManager(enemyManager_) {
	callbackFunctions.emplace(
		CallbackMapKey{ "Beam", "Enemy" },
		CallbackFunctions{
			std::bind(&GameCollisionCallback::beam_enemy_callback, this, __CALLBACK_PLACEHOLDERS_12),
			nullptr,
			nullptr
		}
	);
}

void GameCollisionCallback::beam_enemy_callback(__CALLBACK_ARGUMENT_DEFAULT(enemyCollider, beam)) {
	if (enemyCollider && enemyCollider->group() == "Enemy") {
		enemyManager->callback_collider(enemyCollider);
	}
}
