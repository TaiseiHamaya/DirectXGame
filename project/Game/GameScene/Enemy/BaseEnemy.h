#pragma once

#include <Engine/Module/World/Mesh/MeshInstance.h>

#include <memory>

#include <Engine/Module/World/Collision/Collider/SphereCollider.h>

class BaseEnemy : public MeshInstance {
public:
	BaseEnemy() noexcept(false) = default;
	virtual ~BaseEnemy() noexcept = default;

	BaseEnemy(BaseEnemy&&) noexcept = default;
	BaseEnemy& operator=(BaseEnemy&&) noexcept = default;

public:
	virtual void initialize() override;
	virtual void update() override;

public:
	std::shared_ptr<SphereCollider> get_collider() { return collider; };

protected:
	float timer;
	float ActiveTime;

	bool isDestroy;

	int hitpoint;
	int score;
	std::shared_ptr<SphereCollider> collider;
};

