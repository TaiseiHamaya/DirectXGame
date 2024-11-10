#include "GameCollisionCallback.h"

#include "Game/GameScene/Enemy/BaseEnemy.h"

GameCollisionCallback::GameCollisionCallback() {
	callbackFunctions.emplace(
		CallbackMapKey{ "Beam", "Enemy" },
		CallbackFunctions {
			std::bind(&GameCollisionCallback::beam_enemy_callback, this, std::placeholders::_1, std::placeholders::_2),
			nullptr,
			nullptr
		}
	);
}

void GameCollisionCallback::beam_enemy_callback(const BaseCollider* const beam, const BaseCollider* const enemyCollider) {
	auto parent = enemyCollider->get_parent_address();
	if (parent && enemyCollider->group() == "Enemy") {
		const BaseEnemy* enemy = static_cast<const BaseEnemy*>(parent);
	}
}
