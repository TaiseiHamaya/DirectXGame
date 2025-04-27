#pragma once

#include "./BaseDrawExecutor.h"

#include <execution>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <class Executor, typename T>
concept ConceptExecutor = std::derived_from<Executor, BaseDrawExecutor<T>>;

template<class Executor, typename KeyType = std::string, typename InstanceType = typename Executor::InstanceType>
	requires ConceptExecutor<Executor, InstanceType>
class BaseDrawManager {
protected:
	struct Data {
		std::unordered_set<Reference<const InstanceType>> instances;
		// key : meshID
		std::unordered_map<KeyType, Executor> executors;

		Data() = default;
		Data(Data&&) noexcept = default;
	};

public:
	BaseDrawManager() = default;
	virtual ~BaseDrawManager() = default;

	__CLASS_NON_COPYABLE(BaseDrawManager)

public:
	void initialize(u32 numLayer);
	virtual void make_instancing(u32 layer, const KeyType& meshName, u32 maxInstance) = 0;
	void register_instance(Reference<const InstanceType> instance);
	void unregister_instance(Reference<const InstanceType> instance);
	void transfer();
	void draw_layer(u32 layer);

#ifdef DEBUG_FEATURES_ENABLE
public:
	virtual void debug_gui() {};
#endif // _DEBUG

protected:
	std::vector<Data> drawData;

#ifdef DEBUG_FEATURES_ENABLE
	u32 layer{ 0 };
	KeyType select;
	u32 maxInstance{ 0 };
#endif // _DEBUG
};

template<class Executor, typename KeyType, typename InstanceType>
	requires ConceptExecutor<Executor, InstanceType>
inline void BaseDrawManager<Executor, KeyType, InstanceType>::initialize(u32 numLayer) {
	drawData.resize(numLayer);
}

template<class Executor, typename KeyType, typename InstanceType>
	requires ConceptExecutor<Executor, InstanceType>
inline void BaseDrawManager<Executor, KeyType, InstanceType>::register_instance(Reference<const InstanceType> instance) {
	if (!instance || instance->layer() >= drawData.size()) {
		return;
	}

	drawData[instance->layer()].instances.emplace(instance);
}

template<class Executor, typename KeyType, typename InstanceType>
	requires ConceptExecutor<Executor, InstanceType>
inline void BaseDrawManager<Executor, KeyType, InstanceType>::unregister_instance(Reference<const InstanceType> instance) {
	if (!instance || instance->layer() >= drawData.size()) {
		return;
	}

	drawData[instance->layer()].instances.erase(instance);
}

template<class Executor, typename KeyType, typename InstanceType>
	requires ConceptExecutor<Executor, InstanceType>
inline void BaseDrawManager<Executor, KeyType, InstanceType>::transfer() {
	for (Data& data : drawData) {
		// 全てのExecutorをリセット
		auto depthView = std::views::values(data.executors);
		std::for_each(
			std::execution::par, depthView.begin(), depthView.end(),
			[](Executor& executor) {
			executor.begin();
		});
		std::for_each(
			std::execution::par, data.instances.begin(), data.instances.end(),
			[&executors = data.executors](const Reference<const InstanceType>& instance) {
			const KeyType& id = instance->key_id();
			if (!executors.contains(id)) {
				// 選択したMeshIdがExecutorに存在しない
				return;
			}
			auto& executor = executors[id];
			executor.write_to_buffer(instance);
		});
	}
}

template<class Executor, typename KeyType, typename InstanceType>
	requires ConceptExecutor<Executor, InstanceType>
inline void BaseDrawManager<Executor, KeyType, InstanceType>::draw_layer(u32 layer) {
	if (layer >= drawData.size()) {
		// Layer外をDrawCallしようとした
		return;
	}

	for (const Executor& executor : drawData[layer].executors | std::views::values) {
		executor.draw_command();
	}
}
