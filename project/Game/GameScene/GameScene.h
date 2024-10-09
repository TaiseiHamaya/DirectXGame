#pragma once

#include <Engine/Application/Scene/BaseScene.h>

#include <memory>

#include <Engine/Module/Camera/Camera3D.h>
#include "Game/GameScene/Rail/Rail.h"

class GameScene : public BaseScene {
public:
	GameScene() = default;
	~GameScene() = default;

public:
	void load();
	void initialize();
	void finalize();
	void poped();

	void begin();
	void update();
	void begin_rendering();
	void late_update();

	void draw() const;

#ifdef _DEBUG
public:
	virtual void debug_update();
#endif // _DEBUG

private:
	std::unique_ptr<Camera3D> camera3D;
	std::unique_ptr<Rail> rail;
};
