#pragma once

#include <Engine/Module/World/Mesh/MeshInstance.h>

#include <memory>
#include <string>

#include <Engine/Module/World/Collision/Collider/SphereCollider.h>

struct EnemyTypeData {
	std::string useObj;
	std::string objDirectory;
	float radius;
	int hitpoint;
	int score;
	bool isLookForward;
};

struct EnemyMovementsData {
	std::vector<Vector3> controls;
	float Time;
};

class BaseEnemy : public MeshInstance {
public:
	BaseEnemy() noexcept(false) = default;
	virtual ~BaseEnemy() noexcept = default;

	BaseEnemy(BaseEnemy&&) noexcept = default;
	BaseEnemy& operator=(BaseEnemy&&) noexcept = default;

public:
	void initialize(const Vector3& start_, const EnemyTypeData& typeData, EnemyMovementsData data);
	virtual void update() override;

public:
	void hit();

private:
	void move();

public:
	bool is_dead() const;
	std::shared_ptr<SphereCollider> get_collider() { return collider; };
	const std::string& use_model_name() const { return modelName; };
	int get_score() const { return score; };

protected:
	float timer;
	bool isLookForward;
	Vector3 start;

	int hitpoint;
	std::shared_ptr<SphereCollider> collider;

	int score;
	std::string modelName;

	EnemyMovementsData movements;
};

