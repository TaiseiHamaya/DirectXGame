#include "Rect3dDrawManager.h"

void Rect3dDrawManager::make_instancing(uint32_t layer, const PrimitiveType& primitive, uint32_t maxInstance) {
	if (layer >= drawData.size()) {
		return;
	}

	Data& data = drawData[layer];
	Rect3dDrawExecutor& executor = data.executors[primitive];
	executor.reinitialize(
		primitive, maxInstance
	);
}

void Rect3dDrawManager::make_instancing(uint32_t layer, uint32_t maxInstance) {
	if (layer >= drawData.size()) {
		return;
	}

	Data& data = drawData[layer];
	Rect3dDrawExecutor& executor = data.executors[PrimitiveType::Rect3D];
	executor.reinitialize(
		PrimitiveType::Rect3D, maxInstance
	);
}
