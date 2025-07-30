#include "WorldCluster.h"

void WorldCluster::initialize() {
	worldRoot.initialize();
	worldRenderCollection.initialize();

	worldRoot.setup(instanceBucket);
}

void WorldCluster::update() {
	worldRoot.update();

	instanceBucket.begin_frame();

	worldRenderCollection.transfer();

	worldRoot.post_update();
}

void WorldCluster::end_frame() {
	worldRenderCollection.remove_marked_destroy();

	worldRenderCollection.collect_instantiated(instanceBucket);

	worldRoot.destroy_marked_instances();
}

Reference<WorldRenderCollection> WorldCluster::render_collection() {
	return worldRenderCollection;
}
