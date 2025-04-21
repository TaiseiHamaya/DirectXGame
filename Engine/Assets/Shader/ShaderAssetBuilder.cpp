#include "ShaderAssetBuilder.h"

#include "./ShaderAsset.h"
#include "./ShaderLibrary.h"

#include <Library/Utility/Tools/SmartPointer.h>

ShaderAssetBuilder::ShaderAssetBuilder(const std::filesystem::path& filePath_) {
	filePath = filePath_;
}

void ShaderAssetBuilder::preprocess() {
	asset = eps::CreateShared<ShaderAsset>();
}

bool ShaderAssetBuilder::run() {
	return asset->load(filePath);
}

void ShaderAssetBuilder::postprocess() {
	// Do nothing
}

void ShaderAssetBuilder::transfer() {
	ShaderLibrary::Transfer(filePath.filename().string(), asset);
}
