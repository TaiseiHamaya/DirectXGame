#pragma once

#include "../../BaseResourceBuilder.h"

#include <memory>

class NodeAnimationResource;

class NodeAnimationResourceBuilder final : public BaseResourceBuilder {
public:
	NodeAnimationResourceBuilder(const std::filesystem::path& filePath_);
	~NodeAnimationResourceBuilder() = default;

public:
	void preprocess() override;

	bool run() override;

	void postprocess() override;

	void transfer() override;

private:
	std::shared_ptr<NodeAnimationResource> nodeAnimationData;
};

