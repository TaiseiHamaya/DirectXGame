#ifdef _DEBUG

#include "DebugValues.h"

DebugValues::DebugValues() {
	gridMesh = std::make_unique<MeshInstance>("Grid.obj");
	gridMesh->initialize();
}

DebugValues& DebugValues::GetInsance() {
	static DebugValues instance;
	return instance;
}

void DebugValues::ShowGrid() {
	auto& instance = GetInsance();
	instance.gridMesh->begin_rendering();
	instance.gridMesh->draw();
}
#endif // _DEBUG
