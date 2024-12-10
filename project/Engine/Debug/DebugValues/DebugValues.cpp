#ifdef _DEBUG

#include "DebugValues.h"

DebugValues::DebugValues() {
	gridMesh = std::make_unique<MeshInstance>("Grid.obj");
}

DebugValues& DebugValues::GetInstance() {
	static DebugValues instance;
	return instance;
}

void DebugValues::ShowGrid() {
	auto& instance = GetInstance();
	instance.gridMesh->begin_rendering();
	instance.gridMesh->draw();
}
#endif // _DEBUG
