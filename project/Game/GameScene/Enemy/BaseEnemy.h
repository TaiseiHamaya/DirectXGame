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
	virtual void initialize();
	virtual void update();

protected:
	float timer;
	float ActiveTime;

	bool isDestroy;

	int hitpoint;
	int score;
	std::unique_ptr<SphereCollider> collider;
};

