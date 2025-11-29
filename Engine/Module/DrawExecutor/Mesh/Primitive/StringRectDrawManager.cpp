#include "StringRectDrawManager.h"

void StringRectDrawManager::make_instancing(u32 layer, const BlendMode& blendMode, u32 maxRenderingChar) {
	make_instancing(layer, blendMode, maxRenderingChar, maxRenderingChar * 128);
}

void StringRectDrawManager::make_instancing(u32 layer, const BlendMode& blendMode, u32 maxRenderingChar, u32 maxInstance) {
	auto key = std::make_pair(layer, blendMode);
	if (executors.contains(key)) {
		return;
	}

	// 追加
	StringRectDrawExecutor& executor = executors[key];
	executor.reinitialize(
		blendMode, maxRenderingChar, maxInstance
	);
	layerExecutors[layer].emplace_back(executor);
}