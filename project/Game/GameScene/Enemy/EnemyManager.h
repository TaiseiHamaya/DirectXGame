#pragma once

#include <list>
#include <memory>
#include <unordered_map>

#include <Engine/Module/ParticleSystem/ParticleSystemModel.h>

#include "BaseEnemy.h"
#include "EnemyEditor/EnemyMovementsEditor.h"
#include "EnemyEditor/EnemyTypeEditor.h"

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
	void create(const Vector3& translate, const std::string& type, const std::string& movement);
	void destroy(BaseEnemy* enemy);

public:
	const EnemyTypeEditor* get_type_database() const { return typeDatabase.get(); };
	const EnemyMovementsEditor* get_movement_database() const { return movementDatabase.get(); };

#ifdef _DEBUG
public:
	void debug_gui();
#endif // _DEBUG

private:
	std::list<std::unique_ptr<BaseEnemy>> enemies;
	std::unordered_map<BaseCollider*, BaseEnemy*> reverseEnemies;

	std::list<ParticleSystemModel> deadParticles;

	std::unique_ptr<EnemyTypeEditor> typeDatabase;
	std::unique_ptr<EnemyMovementsEditor> movementDatabase;

	CollisionManager* collisionManager;
	ScoreManager* scoreManager;
};
