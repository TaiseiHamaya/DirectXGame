#pragma once

#include <Engine/Runtime/Scene/BaseScene.h>

#include <memory>

#include "Game/GameScene/Player/RailCamera.h"
#include "Game/GameScene/Rail/Rail.h"
#include "Game/GameScene/Player/Beam.h"

#include <Engine/Module/Render/RenderNode/Object3DNode/Object3DNode.h>
#include <Engine/Module/Render/RenderNode/Sprite/SpriteNode.h>

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
	std::unique_ptr<Beam> beam;

	std::shared_ptr<Object3DNode> object3dNode;
	std::shared_ptr<SpriteNode> spriteNode;
};
