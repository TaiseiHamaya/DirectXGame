#pragma once

#include <optional>
#include <string>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "./BaseRenderPipeline.h"

namespace szg {

class IPostEffectPipeline : public BaseRenderPipeline {
public:
	IPostEffectPipeline() = default;
	virtual ~IPostEffectPipeline() = default;

	SZG_CLASS_MOVE_ONLY(IPostEffectPipeline)

public:
	void setup(const std::optional<std::string>& groupName_);

	virtual void execute_effect_command() = 0;

protected:
	std::optional<std::string> groupName;
};

}; // szg
