#pragma once

#include "../BaseResourceBuilder.h"

#include <memory>

class AudioResource;

class AudioResourceBuilder final : public BaseResourceBuilder {
public:
	AudioResourceBuilder(const std::filesystem::path& filePath_);
	~AudioResourceBuilder() = default;

public:
	void preprocess() override;

	bool run() override;

	void postprocess() override;

	void transfer() override;

private:
	std::unique_ptr<AudioResource> audioData;
};
