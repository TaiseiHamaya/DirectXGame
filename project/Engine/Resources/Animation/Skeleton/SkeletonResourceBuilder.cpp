#include "SkeletonResourceBuilder.h"

#include "./SkeletonManager.h"
#include "./SkeletonResource.h"
#include "Engine/Utility/Tools/SmartPointer.h"

SkeletonResourceBuilder::SkeletonResourceBuilder(const std::filesystem::path& filePath_) {
	filePath = filePath_;
}

void SkeletonResourceBuilder::preprocess() {
	skeletonData = eps::CreateShared<SkeletonResource>();
}

bool SkeletonResourceBuilder::run() {
	return skeletonData->load(filePath);
}

void SkeletonResourceBuilder::postprocess() {
	// Do nothing
}

void SkeletonResourceBuilder::transfer() {
	SkeletonManager::Transfer(filePath.filename().string(), skeletonData);
}
