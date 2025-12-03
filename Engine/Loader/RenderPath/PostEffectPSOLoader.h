#pragma once

#include <memory>
#include <vector>

#include <json.hpp>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "RenderNodeLoader.h"

class RenderTargetCollection;
class PostEffectPSO;

class PostEffectPSOLoader final {
public:
	void setup(Reference<std::vector<RenderNodeLoader::ImmidiateData>> immidiateData_);

	std::unique_ptr<PostEffectPSO> entry_point(const nlohmann::json& json);

private:
	Reference<std::vector<RenderNodeLoader::ImmidiateData>> immidiateData;
};
