#include "Rect3dDrawManager.h"

void Rect3dDrawManager::make_instancing(u32 layer, const PrimitiveType& primitive, u32 maxInstance) {
	if (layer >= drawData.size()) {
		return;
	}

	Data& data = drawData[layer];
	Rect3dDrawExecutor& executor = data.executors[primitive];
	executor.reinitialize(
		primitive, maxInstance
	);
}

void Rect3dDrawManager::make_instancing(u32 layer, u32 maxInstance) {
	if (layer >= drawData.size()) {
		return;
	}

	Data& data = drawData[layer];
	Rect3dDrawExecutor& executor = data.executors[PrimitiveType::Rect3D];
	executor.reinitialize(
		PrimitiveType::Rect3D, maxInstance
	);
}
