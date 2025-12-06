#include "IPostEffectPipeline.h"

using namespace szg;

void IPostEffectPipeline::setup(const std::optional<std::string>& groupName_) {
	groupName = groupName_;
}
