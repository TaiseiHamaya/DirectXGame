#pragma once

#include <Engine/Module/World/Collision/CollisionCallbackManager.h>
#include <Engine/Module/World/Collision/Collider/BaseCollider.h>

class GameCollisionCallback final : public CollisionCallbackManager {
public:
	GameCollisionCallback();
	~GameCollisionCallback() = default;

	__NON_COPYABLE_CLASS(GameCollisionCallback)

public:
	void beam_enemy_callback(const BaseCollider* const beam, const BaseCollider* const enemy);
};
