#pragma once

#include <string>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "../BaseDrawManager.h"
#include "./StaticMeshDrawExecutor.h"

#ifdef DEBUG_FEATURES_ENABLE
class Camera3D;
#endif // _DEBUG
\
class StaticMeshDrawManager final : public BaseDrawManager<StaticMeshDrawExecutor> {
public:
	StaticMeshDrawManager() = default;
	virtual ~StaticMeshDrawManager() = default;

	__CLASS_NON_COPYABLE(StaticMeshDrawManager)

public:
	void make_instancing(uint32_t layer, const std::string& meshName, uint32_t maxInstance);

#ifdef DEBUG_FEATURES_ENABLE
public:
	void register_debug_instance(uint32_t layer, Reference<const Camera3D> camera, bool isShowGrid);
	void debug_gui() override;
#endif // _DEBUG
};
