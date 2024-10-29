#pragma once

#include <Engine/Runtime/Scene/BaseScene.h>

#include <memory>

#include "Game/GameScene/Player/RailCamera.h"
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
	std::unique_ptr<RailCamera> camera3D;
	std::unique_ptr<Rail> rail;
};
