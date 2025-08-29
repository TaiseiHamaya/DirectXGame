#pragma once

#include "./BaseDrawExecutor.h"

#include <execution>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <Library/Utility/Tools/Hash.h>

template <class Executor, typename T>
concept ConceptExecutor = std::derived_from<Executor, BaseDrawExecutor<T>>;

template<class Executor, typename KeyType = std::string, typename InstanceType = typename Executor::InstanceType>
	requires ConceptExecutor<Executor, InstanceType>
class BaseDrawManager {
public:
	BaseDrawManager() = default;
	virtual ~BaseDrawManager() = default;

	__CLASS_NON_COPYABLE(BaseDrawManager)

public:
	void initialize(u32 numLayer);
	virtual void make_instancing(u32 layer, const KeyType& meshName, u32 maxInstance) = 0;
	void register_instance(Reference<const InstanceType> instance);
	void unregister_instance(Reference<const InstanceType> instance);
	void remove_marked_destroy();
	void transfer();
	void draw_layer(u32 layer) const;

#ifdef DEBUG_FEATURES_ENABLE
public:
	virtual void debug_gui() {};
#endif // _DEBUG

protected:
	u32 maxLayer;
	std::unordered_set<Reference<const InstanceType>> instances;
	std::unordered_map<std::pair<u32, KeyType>, Executor> executors;
	std::vector<std::vector<Reference<Executor>>> layerExecutors;

#ifdef DEBUG_FEATURES_ENABLE
	u32 d_layer{ 0 };
	KeyType select;
	u32 d_maxInstance{ 0 };
#endif // _DEBUG
};

template<class Executor, typename KeyType, typename InstanceType>
	requires ConceptExecutor<Executor, InstanceType>
inline void BaseDrawManager<Executor, KeyType, InstanceType>::initialize(u32 numLayer) {
	maxLayer = numLayer;
	layerExecutors.resize(maxLayer);
}

template<class Executor, typename KeyType, typename InstanceType>
	requires ConceptExecutor<Executor, InstanceType>
inline void BaseDrawManager<Executor, KeyType, InstanceType>::register_instance(Reference<const InstanceType> instance) {
	instances.emplace(instance);
}

template<class Executor, typename KeyType, typename InstanceType>
	requires ConceptExecutor<Executor, InstanceType>
inline void BaseDrawManager<Executor, KeyType, InstanceType>::unregister_instance(Reference<const InstanceType> instance) {
	instances.emplace(instance);
}

template<class Executor, typename KeyType, typename InstanceType>
	requires ConceptExecutor<Executor, InstanceType>
inline void BaseDrawManager<Executor, KeyType, InstanceType>::remove_marked_destroy() {
	std::erase_if(instances, [](const Reference<const InstanceType>& instance) {
		return instance->is_marked_destroy();
	});
}

template<class Executor, typename KeyType, typename InstanceType>
	requires ConceptExecutor<Executor, InstanceType>
inline void BaseDrawManager<Executor, KeyType, InstanceType>::transfer() {
	// 全てのExecutorをリセット
	auto executorsValue = std::views::values(executors);
	std::for_each(
		std::execution::par, executorsValue.begin(), executorsValue.end(),
		[](Executor& executor) {
		executor.begin();
	});
	// 全てのExecutorをリセット
	std::for_each(
		std::execution::par, instances.begin(), instances.end(),
		[&](const Reference<const InstanceType> instance) {
		auto key = std::make_pair(instance->layer(), instance->key_id());
		if (!executors.contains(key)) {
			// executorにkeyが存在しない
			return;
		}
		executors.at(key).write_to_buffer(instance);
	});
}

template<class Executor, typename KeyType, typename InstanceType>
	requires ConceptExecutor<Executor, InstanceType>
inline void BaseDrawManager<Executor, KeyType, InstanceType>::draw_layer(u32 layer) const {
	if (layer >= layerExecutors.size()) {
		// Layer外をDrawCallしようとした
		return;
	}

	for (Reference<const Executor> executor : layerExecutors[layer]) {
		executor->draw_command();
	}
}
