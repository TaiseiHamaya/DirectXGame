#include "BaseEnemy.h"

#include <Engine/Runtime/WorldClock/WorldClock.h>
#include <Engine/Utility/Tools/SmartPointer.h>

void BaseEnemy::initialize(const std::string& useObj, float radius_, int hitpoint_, float ActiveTime_, int score_) {
	MeshInstance::initialize();
	collider = eps::CreateShared<SphereCollider>();
	collider->initialize();
	collider->set_radius(radius_);

	reset_object(useObj);
	modelName = useObj;
	hitpoint = hitpoint_;
	ActiveTime = ActiveTime_;
	score = score_;
	collider->set_parent(*this);
}

void BaseEnemy::update() {
	timer += WorldClock::DeltaSeconds();
	if (timer >= ActiveTime) {
		isActive = false;
	}
}

void BaseEnemy::hit() {
	--hitpoint;
}

bool BaseEnemy::is_dead() const {
	return hitpoint <= 0;
}
