#pragma once

#include <list>
#include <memory>

#include "BaseEnemy.h"

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

private:
	std::list<std::unique_ptr<BaseEnemy>> enemies;

	CollisionManager* collisionManager;
};

