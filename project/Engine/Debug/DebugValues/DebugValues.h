#pragma once

#ifdef _DEBUG

#include <memory>

#include <Engine/Module/World/Mesh/StaticMeshInstance.h>

class DebugValues {
private:
	DebugValues();
	~DebugValues() = default;

	DebugValues(const DebugValues&) = delete;
	DebugValues& operator=(const DebugValues&) = delete;

public:
	static DebugValues& GetInstance();

public:
	static Reference<const StaticMeshInstance> GetGridInstance();

private:
	std::unique_ptr<StaticMeshInstance> gridMesh = nullptr;
};

#endif // _DEBUG
