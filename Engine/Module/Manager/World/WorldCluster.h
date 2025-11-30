#pragma once

#include <filesystem>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/Module/Manager/World/InstanceBucket.h"
#include "Engine/Module/Manager/World/WorldRoot.h"
#include "Engine/Module/Render/WorldRenderCollection.h"
#include "Engine/Module/World/Collision/CollisionManager.h"

class WorldCluster final {
public:
	WorldCluster() = default;
	~WorldCluster() = default;

	__CLASS_NON_COPYABLE(WorldCluster)

public:
	void initialize();
	void setup(const std::filesystem::path& setupFile);

	void begin_frame();

	void update();

	void pre_draw();

	void end_frame();

public:
	WorldRoot& world_root_mut();

public:
	Reference<WorldRenderCollection> render_collection();

private:
	WorldRoot worldRoot;
	InstanceBucket instanceBucket;
	WorldRenderCollection worldRenderCollection;
	CollisionManager collisionManager;
};
