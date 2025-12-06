#pragma once

#include "../IAssetBuilder.h"

#include <memory>

namespace szg {

class PolygonMesh;

class PolygonMeshBuilder final : public IAssetBuilder {
public:
	PolygonMeshBuilder(const std::filesystem::path& filePath_);
	~PolygonMeshBuilder() = default;

public:
	bool run() override;

	void postprocess() override;

	void transfer() override;

private:
	std::shared_ptr<PolygonMesh> meshResult;
};


}; // szg
