#include "NodeAnimationAssetBuilder.h"

#include "./NodeAnimationAsset.h"
#include "./NodeAnimationLibrary.h"

#include "Library/Utility/Tools/SmartPointer.h"

NodeAnimationAssetBuilder::NodeAnimationAssetBuilder(const std::filesystem::path& filePath_) {
	filePath = filePath_;
}

void NodeAnimationAssetBuilder::preprocess() {
	nodeAnimationData = eps::CreateShared<NodeAnimationAsset>();
}

bool NodeAnimationAssetBuilder::run() {
	return nodeAnimationData->load(filePath);
}

void NodeAnimationAssetBuilder::postprocess() {
	// Do nothing
}

void NodeAnimationAssetBuilder::transfer() {
	NodeAnimationLibrary::Transfer(filePath.filename().string(), nodeAnimationData);
}
