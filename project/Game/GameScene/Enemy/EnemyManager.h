#pragma once

#include <list>
#include <memory>
#include <unordered_map>

#include "BaseEnemy.h"

class BaseCollider;
class CollisionManager;

class EnemyManager {
public:


public:
	void initialize(CollisionManager* collisionManager_);
	void begin();
	void update();
	void begin_rendering() noexcept;
	void late_update();
	void draw() const;

public:
	void callback_collider(BaseCollider* const collider);
	void create();
	//void destroy();

private:
	std::list<std::unique_ptr<BaseEnemy>> enemies;

	std::unordered_map<BaseCollider*, BaseEnemy*> reverseEnemies;

	CollisionManager* collisionManager;
};
