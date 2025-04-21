#pragma once

#include "../../BaseAssetBuilder.h"

#include <memory>

class SkeletonAsset;

class SkeletonAssetBuilder final : public BaseAssetBuilder {
public:
	SkeletonAssetBuilder(const std::filesystem::path& filePath_);
	~SkeletonAssetBuilder() = default;

public:
	void preprocess() override;

	bool run() override;

	void postprocess() override;

	void transfer() override;

private:
	std::shared_ptr<SkeletonAsset> skeletonData;
};
