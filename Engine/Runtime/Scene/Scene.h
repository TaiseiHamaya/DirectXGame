#pragma once

#include <string>
#include <vector>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/Module/Manager/World/WorldCluster.h"
#include "Engine/Module/Render/RenderDAG.h"

class Scene final {
public:
	Scene() = default;
	~Scene() = default;

	__CLASS_NON_COPYABLE(Scene)

public:
	void initialize(const std::string& sceneName);

	void update();

	void draw() const;

	void end_frame();

public:
	Reference<WorldCluster> get_world(u32 index);

private:
	std::vector<WorldCluster> worlds;
	RenderDAG renderDAG;
};
