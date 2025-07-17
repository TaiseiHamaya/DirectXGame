#include "Rect3dDrawManager.h"

void Rect3dDrawManager::make_instancing(u32 layer, const PrimitiveType& primitive, u32 maxInstance) {
	auto key = std::make_pair(layer, PrimitiveType::Rect3D);
	if (executors.contains(key)) {
		return;
	}

	// 追加
	Rect3dDrawExecutor& executor = executors[key];
	executor.reinitialize(
		primitive, maxInstance
	);
}

void Rect3dDrawManager::make_instancing(u32 layer, u32 maxInstance) {
	auto key = std::make_pair(layer, PrimitiveType::Rect3D);
	if (executors.contains(key)) {
		return;
	}

	// 追加
	Rect3dDrawExecutor& executor = executors[key];
	executor.reinitialize(
		PrimitiveType::Rect3D, maxInstance
	);
	// layer配列に保存
	layerExecutors[layer].emplace_back(executor);
}
