#include "BaseEnemy.h"

#include <Engine/Runtime/WorldClock/WorldClock.h>
#include <Engine/Utility/Tools/SmartPointer.h>

#include "Game/GameScene/Rail/CatmullRom.h"

void BaseEnemy::initialize(const Vector3& start_, const EnemyTypeData& typeData, EnemyMovementsData data) {
	MeshInstance::initialize();
	collider = eps::CreateShared<SphereCollider>();
	collider->initialize();
	collider->set_radius(typeData.radius);

	reset_object(typeData.useObj);
	start = start_;
	modelName = typeData.useObj;
	hitpoint = typeData.hitpoint;
	score = typeData.score;
	collider->set_parent(*this);
	movements = std::move(data);
	timer = 0;
	move();
}

void BaseEnemy::update() {
	timer += WorldClock::DeltaSeconds();
	if (timer >= movements.Time) {
		isActive = false;
		hitpoint = 0;
		return;
	}
	move();
}

void BaseEnemy::hit() {
	--hitpoint;
}

void BaseEnemy::move() {
	Vector3 translate = transform.get_translate();
	transform.set_translate(
		start + CatmullRom(movements.controls, timer / movements.Time)
	);
	Vector3 forward = (transform.get_translate() - translate).normalize();
	transform.set_quaternion(
		Quaternion::LookForward(forward)
	);
}

bool BaseEnemy::is_dead() const {
	return hitpoint <= 0;
}
