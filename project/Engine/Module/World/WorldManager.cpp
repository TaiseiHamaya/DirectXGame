#include "WorldManager.h"

WorldManager::WorldManager() {
	worldInstances.resize(10);
}

WorldManager::~WorldManager() = default;

void WorldManager::update_matrix() {
	for (auto& depth : worldInstances) {
		for (auto& instance : depth) {
			instance->fixed_update();
			instance->update_affine();
		}
	}
}

void WorldManager::reset_depth(Reference<WorldInstance> instance, uint32_t after) {
	worldInstances[instance->depth()].erase(instance);
	worldInstances[after].emplace(instance);
}

void WorldManager::erase(Reference<WorldInstance> instance) {
	worldInstances[instance->depth()].erase(instance);
}
