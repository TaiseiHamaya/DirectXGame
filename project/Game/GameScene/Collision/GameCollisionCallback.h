#pragma once

#include <Engine/Module/World/Collision/CollisionCallbackManager.h>
#include <Engine/Module/World/Collision/Collider/BaseCollider.h>

class EnemyManager;

class GameCollisionCallback final : public CollisionCallbackManager {
public:
	GameCollisionCallback(EnemyManager* enemyManager_);
	~GameCollisionCallback() = default;

	__NON_COPYABLE_CLASS(GameCollisionCallback)

private:
	void beam_enemy_callback(__CALLBACK_ARGUMENT_DEFAULT(enemyCollider, beam));

public:

private:
	EnemyManager* enemyManager;
};
