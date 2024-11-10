#include "BaseEnemy.h"

#include <Engine/Runtime/WorldClock/WorldClock.h>
#include <Engine/Utility/Tools/SmartPointer.h>

void BaseEnemy::initialize() {
	MeshInstance::initialize();
	collider = eps::CreateShared<SphereCollider>();
	collider->initialize();
	collider->set_radius(0.2f);

	reset_object("enemy.obj");
	collider->set_parent(*this);
	ActiveTime = 100;
}

void BaseEnemy::update() {
	timer += WorldClock::DeltaSeconds();
	if (timer >= ActiveTime) {
		isActive = false;
	}
}
