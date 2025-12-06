#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

namespace szg {

class IRenderNode {
public:
	IRenderNode() = default;
	virtual ~IRenderNode() noexcept = default;

	SZG_CLASS_MOVE_ONLY(IRenderNode)

public:
	virtual void stack_command() = 0;
};

}; // szg
