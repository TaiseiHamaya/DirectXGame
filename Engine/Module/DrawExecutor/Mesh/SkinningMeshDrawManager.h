#pragma once

#include <string>

#include "../BaseDrawManager.h"
#include "./SkinningMeshDrawExecutor.h"
#include "Engine/Module/World/Mesh/SkinningMeshInstance.h"

class SkinningMeshDrawManager final : public BaseDrawManager<SkinningMeshDrawExecutor> {
public:
	SkinningMeshDrawManager() = default;
	virtual ~SkinningMeshDrawManager() = default;

	__CLASS_NON_COPYABLE(SkinningMeshDrawManager)

public:
	void make_instancing(u32 layer, const std::string& meshName, u32 maxInstance);

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();
#endif // _DEBUG
};
