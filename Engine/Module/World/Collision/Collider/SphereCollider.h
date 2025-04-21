#pragma once

#include "BaseCollider.h"

class SphereCollider : public BaseCollider {
public:
	SphereCollider() = default;
	SphereCollider(float radius_);
	~SphereCollider() = default;

public:
	void set_radius(float radius_);
	float get_radius() const;

private:
	float radius{ 1 };
};
