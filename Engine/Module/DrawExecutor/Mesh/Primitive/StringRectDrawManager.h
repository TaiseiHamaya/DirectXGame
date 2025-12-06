#pragma once

#include "../../BaseDrawManager.h"
#include "./StringRectDrawExecutor.h"

#include "Engine/Module/World/Mesh/Primitive/StringRectInstance.h"

namespace szg {

class StringRectDrawManager : public BaseDrawManager<StringRectDrawExecutor, BlendMode> {
public:
	StringRectDrawManager() = default;
	virtual ~StringRectDrawManager() = default;

	SZG_CLASS_MOVE_ONLY(StringRectDrawManager)

public:
	void make_instancing(u32 layer, const BlendMode& blendMode, u32 maxRenderingChar) override;
	void make_instancing(u32 layer, const BlendMode& blendMode, u32 maxRenderingChar, u32 maxInstance);
};

}; // szg
