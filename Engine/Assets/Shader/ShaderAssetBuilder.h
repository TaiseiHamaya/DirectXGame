#pragma once

#include "../IAssetBuilder.h"

#include <memory>

namespace szg {

class ShaderAsset;

class ShaderAssetBuilder final : public IAssetBuilder {
public:
	ShaderAssetBuilder(const std::filesystem::path& filePath_);
	~ShaderAssetBuilder() = default;

public:
	bool run() override;

	void postprocess() override;

	void transfer() override;

private:
	std::shared_ptr<ShaderAsset> asset;
};

}; // szg
