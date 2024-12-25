#pragma once

#include "../../BaseResourceBuilder.h"

#include <memory>

class SkeletonResource;

class SkeletonResourceBuilder final : public BaseResourceBuilder {
public:
	SkeletonResourceBuilder(const std::filesystem::path& filePath_);
	~SkeletonResourceBuilder() = default;

public:
	void preprocess() override;

	bool run() override;

	void postprocess() override;

	void transfer() override;

private:
	std::shared_ptr<SkeletonResource> skeletonData;
};
