#include "WorldRoot.h"

WorldRoot::WorldRoot() = default;
WorldRoot::~WorldRoot() = default;

void WorldRoot::initialize() {
}

void WorldRoot::setup(Reference<InstanceBucket> instanceBucket_) {
	instanceBucket = instanceBucket_;
}

void WorldRoot::update() {
}

void WorldRoot::post_update() {
	// Destroy instances
	for(auto& instance : destroyInstances) {
		Reference<const WorldInstance> parent = instance->get_parent_address();
		//worldInstances.at(instance->get_instance_id())->destroyCallback();
	}
}

void WorldRoot::destroy(Reference<WorldInstance> instance) {
	destroyInstances.emplace_back(instance);
}

void WorldRoot::destroy_marked_instances() {
}
