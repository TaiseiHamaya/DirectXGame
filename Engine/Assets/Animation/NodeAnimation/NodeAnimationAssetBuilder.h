#pragma once

#include "../../BaseAssetBuilder.h"

#include <memory>

class NodeAnimationAsset;

class NodeAnimationAssetBuilder final : public BaseAssetBuilder {
private:
	struct BuildData {
		std::string name;
		std::shared_ptr<NodeAnimationAsset> asset;
	};

public:
	NodeAnimationAssetBuilder(const std::filesystem::path& filePath_);
	~NodeAnimationAssetBuilder() = default;

public:
	void preprocess() override;

	bool run() override;

	void postprocess() override;

	void transfer() override;

private:
	std::vector<BuildData> nodeAnimationData;
};

