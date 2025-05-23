#pragma once

#include <vector>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/Module/World/WorldManager.h"

#include "BaseScene.h"

class Scene final : public BaseScene {
public:
	Scene() = default;
	~Scene() = default;

	__CLASS_NON_COPYABLE(Scene)

public:
	//void load(std::filesystem::path assetCollection);
	void initialize();

	void begin();
	void update();
	void transfer();
	void late_update();

	void draw() const;

private:
	//std::vector<WorldManager> worlds;
};
