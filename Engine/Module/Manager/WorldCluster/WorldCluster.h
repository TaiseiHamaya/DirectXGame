#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/Module/World/InstanceBucket.h"
#include "Engine/Module/World/WorldRoot.h"
#include "Engine/Module/Render/WorldRenderCollection.h"

class WorldCluster final {
public:
	WorldCluster() = default;
	~WorldCluster() = default;

	__CLASS_NON_COPYABLE(WorldCluster)

public:
	void initialize();
	void update();
	void end_frame();

public:
	Reference<WorldRenderCollection> render_collection();

private:
	WorldRoot worldRoot;
	InstanceBucket instanceBucket;
	WorldRenderCollection worldRenderCollection;
};
