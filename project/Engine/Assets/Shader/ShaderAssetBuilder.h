#pragma once

#include "../BaseAssetBuilder.h"

#include <memory>

class ShaderAsset;

class ShaderAssetBuilder final : public BaseAssetBuilder {
public:
	ShaderAssetBuilder(const std::filesystem::path& filePath_);
	~ShaderAssetBuilder() = default;

public:
	void preprocess() override;

	bool run() override;

	void postprocess() override;

	void transfer() override;

private:
	std::shared_ptr<ShaderAsset> asset;
};
