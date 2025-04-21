#pragma once

#include "../BaseAssetBuilder.h"

#include <memory>

class AudioAsset;

class AudioAssetBuilder final : public BaseAssetBuilder {
public:
	AudioAssetBuilder(const std::filesystem::path& filePath_);
	~AudioAssetBuilder() = default;

public:
	void preprocess() override;

	bool run() override;

	void postprocess() override;

	void transfer() override;

private:
	std::unique_ptr<AudioAsset> audioData;
};
