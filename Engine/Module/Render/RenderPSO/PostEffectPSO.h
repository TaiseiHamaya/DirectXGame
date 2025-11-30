#pragma once

#include <optional>
#include <string>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "./BaseRenderNode.h"

enum class PostEffectType {
	None,
	ChromaticAberration,
	Grayscale,
	Outline,
	RadialBlur,
	DownSampling,
	TextureBlend2,
	TextureBlend4,
};

class PostEffectPSO : public BaseRenderNode {
public:
	PostEffectPSO() = default;
	virtual ~PostEffectPSO() = default;

	__CLASS_NON_COPYABLE(PostEffectPSO)

public:
	void setup(const std::optional<std::string>& groupName_);

	virtual void execute_effect_command() = 0;

protected:
	std::optional<std::string> groupName;
};
