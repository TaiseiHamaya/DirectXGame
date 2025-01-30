#include "PolygonMeshBuilder.h"

#include "./PolygonMesh.h"
#include "./PolygonMeshLibrary.h"

#include <Library/Utility/Tools/SmartPointer.h>

PolygonMeshBuilder::PolygonMeshBuilder(const std::filesystem::path& filePath_) {
	filePath = filePath_;
}

void PolygonMeshBuilder::preprocess() {
	meshData = eps::CreateShared<PolygonMesh>();
}

bool PolygonMeshBuilder::run() {
	return meshData->load(filePath);
}

void PolygonMeshBuilder::postprocess() {
	// Do nothing
}

void PolygonMeshBuilder::transfer() {
	PolygonMeshLibrary::Transfer(filePath.filename().string(), meshData);
}
