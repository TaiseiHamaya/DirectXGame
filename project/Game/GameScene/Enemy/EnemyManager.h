#pragma once

#include <list>
#include <memory>
#include <unordered_map>

#include <Engine/Module/ParticleSystem/ParticleSystemModel.h>

#include "BaseEnemy.h"

class BaseCollider;
class CollisionManager;
class ScoreManager;

class EnemyManager {
private:

public:


public:
	void initialize(
		CollisionManager* collisionManager_,
		ScoreManager* scoreManager_
	);
	void begin();
	void update();
	void begin_rendering() noexcept;
	void late_update();
	void draw() const;

public:
	void callback_collider(BaseCollider* const collider);
	void create();
	void destroy(BaseEnemy* enemy);

private:
	std::list<std::unique_ptr<BaseEnemy>> enemies;
	std::unordered_map<BaseCollider*, BaseEnemy*> reverseEnemies;

	std::list<ParticleSystemModel> deadParticles;

	CollisionManager* collisionManager;
	ScoreManager* scoreManager;
};
