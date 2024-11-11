#pragma once

#include "BaseCollider.h"

class CapsuleCollider : public BaseCollider {
public:
	CapsuleCollider() = default;
	~CapsuleCollider() = default;

public:
	void initialize() override;
	void initialize(float radius_, float length_, const Vector3& direction_);

public:
	void set_radius(float radius_) { radius = radius_; };
	void set_length(float length_) { length = length_; };
	void set_direction(const Vector3& direction_) { direction = direction_; };
	float get_radius() const { return radius; };
	float get_length() const { return length; };
	const Vector3& get_direction() const { return direction; };

public:
	constexpr std::string type() const override;

private:
	float radius;
	float length;
	Vector3 direction;
};

constexpr std::string CapsuleCollider::type() const {
	return "Capsule";
}
