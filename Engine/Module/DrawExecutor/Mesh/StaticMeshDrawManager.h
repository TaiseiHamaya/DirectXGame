#pragma once

#include <string>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "../BaseDrawManager.h"
#include "./StaticMeshDrawExecutor.h"
#include "Engine/Module/World/Mesh/StaticMeshInstance.h"

#ifdef DEBUG_FEATURES_ENABLE
class Camera3D;
#endif // _DEBUG

class StaticMeshDrawManager final : public BaseDrawManager<StaticMeshDrawExecutor> {
public:
	StaticMeshDrawManager() = default;
	virtual ~StaticMeshDrawManager() = default;

	__CLASS_NON_COPYABLE(StaticMeshDrawManager)

public:
	void make_instancing(u32 layer, const std::string& meshName, u32 maxInstance);

#ifdef DEBUG_FEATURES_ENABLE
public:
	void register_debug_instance(u32 layer, Reference<const Camera3D> camera, bool isShowGrid);
	void debug_gui() override;
#endif // _DEBUG
};
