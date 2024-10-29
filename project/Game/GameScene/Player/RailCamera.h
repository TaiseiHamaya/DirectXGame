#pragma once

#include <Engine/Module/World/Camera/Camera3D.h>

class Rail;

class RailCamera : public Camera3D {
public:
	void update();

public:
	void set_rail(const Rail* rail_) { rail = rail_; };

#ifdef _DEBUG
public:
	void debug_gui() override;
#endif // _DEBUG

private:
	Vector3 offset{CVector3::BASIS_Y};

	float mileage;
	float speed;

	const Rail* rail = nullptr;
};
