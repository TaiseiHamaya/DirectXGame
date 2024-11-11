#pragma once

#include <Engine/Module/World/Mesh/MeshInstance.h>

#include <memory>
#include <string>

#include <Engine/Module/World/Collision/Collider/SphereCollider.h>

class BaseEnemy : public MeshInstance {
public:
	BaseEnemy() noexcept(false) = default;
	virtual ~BaseEnemy() noexcept = default;

	BaseEnemy(BaseEnemy&&) noexcept = default;
	BaseEnemy& operator=(BaseEnemy&&) noexcept = default;

public:
	void initialize(const std::string& useObj, float radius_, int hitpoint_, float ActiveTime_, int score_);
	virtual void update() override;

public:
	void hit();

public:
	bool is_dead() const;
	std::shared_ptr<SphereCollider> get_collider() { return collider; };
	const std::string& use_model_name() const { return modelName; };
	int get_score() const { return score; };

protected:
	float timer;
	float ActiveTime;

	int hitpoint;
	std::shared_ptr<SphereCollider> collider;

	int score;
	std::string modelName;
};

