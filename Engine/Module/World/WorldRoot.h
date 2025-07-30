#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "./InstanceBucket.h"
#include "Engine/Application/Output.h"
#include "Engine/Module/World/WorldInstance/WorldInstance.h"

class WorldRoot final {
public:
	WorldRoot();
	~WorldRoot();

	__CLASS_NON_COPYABLE(WorldRoot)

public:
	void initialize();
	void setup(Reference<InstanceBucket> instanceBucket_);

	void update();

	void post_update();

	template<std::derived_from<WorldInstance> T, typename ...Args>
	Reference<T> instantiate(Reference<const WorldInstance> parent = nullptr, Args&&... args);

	void destroy(Reference<WorldInstance> instance);

	void destroy_marked_instances();

private:
	u64 nextInstanceId = 0;
	std::unordered_map<u64, std::unique_ptr<WorldInstance>> worldInstances;

	std::vector<Reference<WorldInstance>> destroyInstances;

	Reference<InstanceBucket> instanceBucket;
};

template<std::derived_from<WorldInstance> T, typename ...Args>
inline Reference<T> WorldRoot::instantiate(Reference<const WorldInstance> parent, Args&&... args) {
	std::unique_ptr<T> instance = std::make_unique<T>(std::forward<Args>(args)...);
	Reference<T> result = instance;
	auto [_, emplaced] = worldInstances.try_emplace(nextInstanceId, std::move(instance));
	if (!emplaced) {
		Warning("Instantiate failed for an unknown reason. Type-\'{}\'", typeid(T).name());
	}
	++nextInstanceId;

	if (parent) {
		result->reparent(parent, false);
	}
	else {
		result->reparent(nullptr, false);
	}

	instanceBucket->register_instance(result);

	return result;
}
