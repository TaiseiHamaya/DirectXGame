#pragma once

#include <memory>
#include <unordered_set>
#include <vector>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/Module/World/WorldInstance/WorldInstance.h"

namespace szg {

class WorldInstance;

class WorldManager {
public:
	WorldManager();
	~WorldManager();

	SZG_CLASS_MOVE_ONLY(WorldManager)

public:
	void update_matrix();

	void reset_depth(Reference<WorldInstance> instance, u32 after);

	void erase(Reference<WorldInstance> instance);

	template<std::derived_from<WorldInstance> T, typename ...Args>
	[[nodiscard]] std::unique_ptr<T> create(Reference<WorldInstance> parent = nullptr, Args&&... args);

	std::vector<std::unordered_set<Reference<WorldInstance>>>& get_world_instances();
	const std::vector<std::unordered_set<Reference<WorldInstance>>>& get_world_instances() const;

private:
	std::vector<std::unordered_set<Reference<WorldInstance>>> worldInstances;
};

template<std::derived_from<WorldInstance> T, typename ...Args>
inline std::unique_ptr<T> WorldManager::create(Reference<WorldInstance> parent, Args&&... args) {
	std::unique_ptr<T> instance = std::make_unique<T>(args...);
	Reference<WorldInstance> ref = instance;
	ref->set_world_manager(this);
	ref->reparent(parent, false);
	return std::move(instance);
}

}; // szg
