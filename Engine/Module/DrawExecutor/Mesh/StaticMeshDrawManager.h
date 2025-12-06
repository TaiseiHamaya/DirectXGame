#pragma once

#include <string>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "../BaseDrawManager.h"
#include "./StaticMeshDrawExecutor.h"
#include "Engine/Module/World/Mesh/StaticMeshInstance.h"

namespace szg {

class StaticMeshDrawManager final : public BaseDrawManager<StaticMeshDrawExecutor> {
public:
	StaticMeshDrawManager() = default;
	virtual ~StaticMeshDrawManager() = default;

	SZG_CLASS_MOVE_ONLY(StaticMeshDrawManager)

public:
	void make_instancing(u32 layer, const std::string& meshName, u32 maxInstance);

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui() override;
#endif // _DEBUG
};

}; // szg
