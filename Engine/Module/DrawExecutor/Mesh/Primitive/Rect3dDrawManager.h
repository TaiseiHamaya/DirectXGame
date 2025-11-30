#pragma once

#include "../../BaseDrawManager.h"
#include "./Rect3dDrawExecutor.h"

#include "Engine/Module/World/Mesh/Primitive/IPrimitiveInstance.h"
#include "Engine/Module/World/Mesh/Primitive/Rect3d.h"

class Rect3dDrawManager : public BaseDrawManager<Rect3dDrawExecutor, BlendMode> {
public:
	Rect3dDrawManager() = default;
	virtual ~Rect3dDrawManager() = default;

	__CLASS_NON_COPYABLE(Rect3dDrawManager)

public:
	void make_instancing(u32 layer, const BlendMode& blendMode, u32 maxInstance) override;
};
