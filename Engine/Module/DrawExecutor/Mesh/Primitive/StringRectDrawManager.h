#pragma once

#include "../../BaseDrawManager.h"
#include "./StringRectDrawExecutor.h"

#include "Engine/Module/World/Mesh/Primitive/IPrimitiveInstance.h"
#include "Engine/Module/World/Mesh/Primitive/StringRectInstance.h"

class StringRectDrawManager : public BaseDrawManager<StringRectDrawExecutor, BlendMode> {
public:
	StringRectDrawManager() = default;
	virtual ~StringRectDrawManager() = default;

	__CLASS_NON_COPYABLE(StringRectDrawManager)

public:
	void make_instancing(u32 layer, const BlendMode& blendMode, u32 maxRenderingChar) override;
	void make_instancing(u32 layer, const BlendMode& blendMode, u32 maxRenderingChar, u32 maxInstance);
};
