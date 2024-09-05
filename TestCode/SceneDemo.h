#pragma once

#include "Engine/Game/Scene/BaseScene.h"

#include <memory>

class GameObject;
class Camera3D;

class SceneDemo : public BaseScene {
public:
	SceneDemo();
	~SceneDemo();

public:
	void load();
	void initialize();
	void finalize();

	void begin() override;
	void update() override;
	void begin_rendering() override;
	void late_update() override;

	void draw() const override;

#ifdef _DEBUG
	void debug_update() override;
#endif // _DEBUG

private:
	std::unique_ptr<GameObject> parent;
	std::unique_ptr<GameObject> child;

	std::unique_ptr<Camera3D> camera3D;
};
