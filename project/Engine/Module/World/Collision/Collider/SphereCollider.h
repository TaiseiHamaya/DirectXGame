#pragma once

#include "BaseCollider.h"

class SphereCollider : public BaseCollider {
public:
	SphereCollider() = default;
	~SphereCollider() = default;

public:
	void initialize() override;

public:
	void set_radius(float radius_);
	float get_radius() const;

private:
	float radius;
};
