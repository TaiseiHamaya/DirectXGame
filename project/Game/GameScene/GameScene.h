#pragma once

#include <Engine/Runtime/Scene/BaseScene.h>

#include <memory>

#include "Enemy/EnemyManager.h"
#include "Player/Beam.h"
#include "Player/RailCamera.h"
#include "Rail/Rail.h"
#include "ScoreManager/ScoreManager.h"
#include "Timeline/Timeline.h"

#include <Engine/Module/World/Collision/CollisionManager.h>
#include <Engine/Module/World/Light/DirectionalLight/DirectionalLightInstance.h>

#include <Engine/Module/Render/RenderPath/RenderPath.h>

class GameScene : public BaseScene {
public:
	GameScene() = default;
	~GameScene() = default;

public:
	void load();
	void initialize();
	void finalize();
	void popped();

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
	std::unique_ptr<RenderPath> renderPath;
	std::unique_ptr<CollisionManager> collisionManager;
	std::unique_ptr<ScoreManager> scoreManager;
	std::unique_ptr<Timeline> timeline;

	std::unique_ptr<DirectionalLightInstance> directionalLight;

	std::unique_ptr<RailCamera> camera3D;
	std::unique_ptr<Rail> rail;
	std::unique_ptr<Beam> beam;
	std::unique_ptr<EnemyManager> enemyManager;
};
