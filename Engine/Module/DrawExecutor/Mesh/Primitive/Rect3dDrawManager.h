#pragma once

#include "../../BaseDrawManager.h"
#include "./Rect3dDrawExecutor.h"

#include "Engine/Module/World/Mesh/Primitive/IPrimitiveInstance.h"

class Rect3dDrawManager : public BaseDrawManager<Rect3dDrawExecutor, PrimitiveType> {
public:
	Rect3dDrawManager() = default;
	virtual ~Rect3dDrawManager() = default;

	__CLASS_NON_COPYABLE(Rect3dDrawManager)

private:
	void make_instancing(uint32_t layer, const PrimitiveType& primitive, uint32_t maxInstance) override;

public:
	void make_instancing(uint32_t layer, uint32_t maxInstance);
};
