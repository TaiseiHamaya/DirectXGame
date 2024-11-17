#pragma once

#include <Engine/Module/World/Camera/Camera3D.h>

class Rail;

class RailCamera : public Camera3D {
public:
	void update();

public:
	void set_rail(const Rail* rail_) { rail = rail_; };

public:
	Vector3 camera_upward_point(const Vector3& offset_) const;
	float get_mileage() const { return mileage; };

#ifdef _DEBUG
public:
	void debug_gui() override;
#endif // _DEBUG

private:
	Vector3 offset{ 0.0f,0.5f,0.0f };
	Vector3 basePoint;

	float mileage;
	float speed;

	const Rail* rail = nullptr;
};
