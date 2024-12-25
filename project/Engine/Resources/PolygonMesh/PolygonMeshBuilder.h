#pragma once

#include "../BaseResourceBuilder.h"

#include <memory>

class PolygonMesh;

class PolygonMeshBuilder final : public BaseResourceBuilder {
public:
	PolygonMeshBuilder(const std::filesystem::path& filePath_);
	~PolygonMeshBuilder() = default;

public:
	void preprocess() override;

	bool run() override;

	void postprocess() override;

	void transfer() override;

private:
	std::shared_ptr<PolygonMesh> meshData;
};

