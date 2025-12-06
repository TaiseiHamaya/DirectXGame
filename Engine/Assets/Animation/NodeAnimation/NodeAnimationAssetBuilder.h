#pragma once

#include "../../IAssetBuilder.h"

#include <memory>

namespace szg {

class NodeAnimationAsset;

class NodeAnimationAssetBuilder final : public IAssetBuilder {
private:
	struct BuildData {
		std::string name;
		std::shared_ptr<NodeAnimationAsset> asset;
	};

public:
	NodeAnimationAssetBuilder(const std::filesystem::path& filePath_);
	~NodeAnimationAssetBuilder() = default;

public:
	bool run() override;

	void postprocess() override;

	void transfer() override;

private:
	std::vector<BuildData> nodeAnimationData;
};


}; // szg
