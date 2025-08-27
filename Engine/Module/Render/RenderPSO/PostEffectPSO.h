#pragma once

#include <string>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "./BaseRenderNode.h"

class PostEffectPSO : public BaseRenderNode {
public:
	PostEffectPSO() = default;
	virtual ~PostEffectPSO() = default;

	__CLASS_NON_COPYABLE(PostEffectPSO)

public:
	void setup(const std::string& groupName_);

	virtual void execute_effect_command() = 0;

protected:
	std::string groupName;
};
