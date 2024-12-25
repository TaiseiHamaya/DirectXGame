#include "NodeAnimationResourceBuilder.h"

#include "Engine/Utility/Tools/SmartPointer.h"
#include "NodeAnimationManager.h"
#include "NodeAnimationResource.h"

NodeAnimationResourceBuilder::NodeAnimationResourceBuilder(const std::filesystem::path& filePath_) {
	filePath = filePath_;
}

void NodeAnimationResourceBuilder::preprocess() {
	nodeAnimationData = eps::CreateShared<NodeAnimationResource>();
}

bool NodeAnimationResourceBuilder::run() {
	return nodeAnimationData->load(filePath);
}

void NodeAnimationResourceBuilder::postprocess() {
	// Do nothing
}

void NodeAnimationResourceBuilder::transfer() {
	NodeAnimationManager::Transfer(filePath.filename().string(), nodeAnimationData);
}
