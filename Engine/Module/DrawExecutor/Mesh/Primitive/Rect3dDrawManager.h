#pragma once

#include "../../BaseDrawManager.h"
#include "./Rect3dDrawExecutor.h"

#include "Engine/Module/World/Mesh/Primitive/IPrimitiveInstance.h"
#include "Engine/Module/World/Mesh/Primitive/Rect3d.h"

class Rect3dDrawManager : public BaseDrawManager<Rect3dDrawExecutor, PrimitiveType> {
public:
	Rect3dDrawManager() = default;
	virtual ~Rect3dDrawManager() = default;

	__CLASS_NON_COPYABLE(Rect3dDrawManager)

private:
	void make_instancing(u32 layer, const PrimitiveType& primitive, u32 maxInstance) override;

public:
	void make_instancing(u32 layer, u32 maxInstance);
};
