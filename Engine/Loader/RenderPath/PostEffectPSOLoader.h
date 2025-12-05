#pragma once

#include <memory>
#include <vector>

#include <json.hpp>

#include <Library/Utility/Template/Reference.h>

#include "RenderNodeLoader.h"

class RenderTargetCollection;
class IPostEffectPipeline;

class PostEffectPSOLoader final {
public:
	void setup(Reference<std::vector<RenderNodeLoader::ImmidiateData>> immediateData_);

	std::unique_ptr<IPostEffectPipeline> entry_point(const nlohmann::json& json);

private:
	Reference<std::vector<RenderNodeLoader::ImmidiateData>> immediateData;
};
