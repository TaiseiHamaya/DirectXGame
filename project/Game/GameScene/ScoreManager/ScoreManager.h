#pragma once

#include <list>
#include <memory>

#include <Engine/Module/World/Mesh/MeshInstance.h>
#include <Engine/Module/World/Sprite/SpriteInstance.h>

class BaseEnemy;
class WorldInstance;

class ScoreManager {
private:
	struct ScoreDisplay {
		std::unique_ptr<MeshInstance> model;
		std::vector<std::unique_ptr<SpriteInstance>> scoreSprite;
	};

public:
	void initialize(const WorldInstance* camera_);
	void begin();
	void update();
	void begin_rendering() noexcept;
	void late_update();
	void draw() const;
	void draw_sprite() const;

public:
	void register_enemy(BaseEnemy* enemy);

private:
	float displayTimer = 0;
	float DefaultDisplayTime = 3.0f;
	std::list<ScoreDisplay> score;

	const WorldInstance* camera;
};
