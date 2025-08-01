#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "./SingleRenderTargetNode.h"

class PostEffectPSO : public SingleRenderTargetNode {
public:
	PostEffectPSO() = default;
	virtual ~PostEffectPSO() = default;

	__CLASS_NON_COPYABLE(PostEffectPSO)

public:
	virtual void execute_effect_command() = 0;
};
