#pragma once

#include "../../IAssetBuilder.h"

#include <memory>

class SkeletonAsset;

/// <summary>
/// Skeletonアセットロード用一時クラス
/// </summary>
class SkeletonAssetBuilder final : public IAssetBuilder {
public:
	SkeletonAssetBuilder(const std::filesystem::path& filePath_);
	~SkeletonAssetBuilder() = default;

public:
	bool run() override;

	void postprocess() override;

	void transfer() override;

private:
	std::shared_ptr<SkeletonAsset> skeletonData;
};
