#include "SkeletonAssetBuilder.h"

#include "./SkeletonAsset.h"
#include "./SkeletonLibrary.h"

#include <Library/Utility/Tools/SmartPointer.h>

SkeletonAssetBuilder::SkeletonAssetBuilder(const std::filesystem::path& filePath_) {
	filePath = filePath_;
}

void SkeletonAssetBuilder::preprocess() {
	skeletonData = eps::CreateShared<SkeletonAsset>();
}

bool SkeletonAssetBuilder::run() {
	return skeletonData->load(filePath);
}

void SkeletonAssetBuilder::postprocess() {
	// Do nothing
}

void SkeletonAssetBuilder::transfer() {
	SkeletonLibrary::Transfer(filePath.filename().string(), skeletonData);
}
