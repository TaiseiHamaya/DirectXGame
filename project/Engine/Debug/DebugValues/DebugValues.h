#pragma once

#ifdef _DEBUG

#include <memory>

#include <Engine/Module/World/Mesh/MeshInstance.h>

class DebugValues {
private:
	DebugValues();
	~DebugValues() = default;

	DebugValues(const DebugValues&) = delete;
	DebugValues& operator=(const DebugValues&) = delete;

public:
	static DebugValues& GetInsance();

public:
	static void ShowGrid();

private:
	std::unique_ptr<MeshInstance> gridMesh = nullptr;
};

#endif // _DEBUG
