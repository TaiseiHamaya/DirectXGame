#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

class IRenderNode {
public:
	IRenderNode() = default;
	virtual ~IRenderNode() noexcept = default;

	__CLASS_NON_COPYABLE(IRenderNode)

public:
	virtual void stack_command() = 0;
};
