#pragma once

#include "Engine/Game/Scene/BaseScene.h"

#include <memory>

class GameObject;

class SceneDemo : public BaseScene {
public:
	SceneDemo();
	~SceneDemo();

public:
	void initialize() override;

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
};
