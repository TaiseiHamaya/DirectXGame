#pragma once

#include <string>

#include "../BaseDrawManager.h"
#include "./SkinningMeshDrawExecutor.h"

class SkinningMeshDrawManager final : public BaseDrawManager<SkinningMeshDrawExecutor> {
public:
	SkinningMeshDrawManager() = default;
	virtual ~SkinningMeshDrawManager() = default;

	__CLASS_NON_COPYABLE(SkinningMeshDrawManager)

public:
	void make_instancing(uint32_t layer, const std::string& meshName, uint32_t maxInstance);

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();
#endif // _DEBUG
};
