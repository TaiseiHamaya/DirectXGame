#pragma once

#include <memory>
#include <unordered_set>
#include <vector>

#include <Library/Utility/Template/Reference.h>

#include "Engine/Module/World/WorldInstance/WorldInstance.h"

class WorldInstance;

class WorldManager {
public:
	WorldManager();
	~WorldManager();

	void update_matrix();

	void reset_depth(Reference<WorldInstance> instance, uint32_t after);

	void erase(Reference<WorldInstance> instance);

	template<std::derived_from<WorldInstance> T, typename ...Args>
	std::unique_ptr<T> create(Reference<const WorldInstance> parent = nullptr, bool isKeepPose = true, Args&&... args);

private:
	std::vector<std::unordered_set<Reference<WorldInstance>>> worldInstances;
};

template<std::derived_from<WorldInstance> T, typename ...Args>
inline std::unique_ptr<T> WorldManager::create(Reference<const WorldInstance> parent, bool isKeepPose, Args&&... args) {
	std::unique_ptr<T> instance = std::make_unique<T>(args...);
	Reference<WorldInstance> ref = instance;
	ref->set_world_manager(this);
	ref->reparent(parent, isKeepPose);
	return std::move(instance);
}
