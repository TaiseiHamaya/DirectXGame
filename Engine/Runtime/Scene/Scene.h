#pragma once

#include <vector>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/Module/Manager/WorldCluster/WorldCluster.h"

class Scene final {
public:
	Scene() = default;
	~Scene() = default;

	__CLASS_NON_COPYABLE(Scene)

public:
	void initialize();

	void update();

	void draw() const;

	void end_frame();

private:
	std::vector<WorldCluster> worlds;
};
