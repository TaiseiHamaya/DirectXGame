#pragma once

#include <Library/Math/Vector2.h>
#include <Library/Math/Vector3.h>
#include <Engine/Module/World/Sprite/SpriteInstance.h>
#include <Engine/Module/World/Mesh/MeshInstance.h>

class RailCamera;

class Beam {
public:

public:
	void initialize();
	void begin();
	void update();
	void begin_rendering();
	void late_update();
	void draw() const;
	void draw_reticle() const;

private:
	void input();

public:
	void set_camera(const RailCamera* camera_);

#ifdef _DEBUG
public:
	void debug_gui();
#endif // _DEBUG

private:
	struct Ray {
		Vector3 origin;
		Vector3 direction;
	};

	Vector2 keyInput;

	Vector2 reticle;
	std::unique_ptr<SpriteInstance> sprite;
	std::unique_ptr<MeshInstance> beam;
	const RailCamera* camera;
	bool isBeaming;

	Vector3 initialPointOffset{ 0.0f,-0.35f,1.5f };
	float reticleMoveSpeed{2.0f};

	Ray beamRay;

};
