#pragma once

#include "../IAssetBuilder.h"

#include <memory>

namespace szg {

class AudioAsset;

class AudioAssetBuilder final : public IAssetBuilder {
public:
	AudioAssetBuilder(const std::filesystem::path& filePath_);
	~AudioAssetBuilder() = default;

public:
	bool run() override;

	void postprocess() override;

	void transfer() override;

private:
	std::unique_ptr<AudioAsset> audioData;
};

}; // szg
