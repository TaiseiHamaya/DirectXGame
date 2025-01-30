#pragma once

#include "../../BaseAssetBuilder.h"

#include <memory>

class NodeAnimationAsset;

class NodeAnimationAssetBuilder final : public BaseAssetBuilder {
public:
	NodeAnimationAssetBuilder(const std::filesystem::path& filePath_);
	~NodeAnimationAssetBuilder() = default;

public:
	void preprocess() override;

	bool run() override;

	void postprocess() override;

	void transfer() override;

private:
	std::shared_ptr<NodeAnimationAsset> nodeAnimationData;
};

