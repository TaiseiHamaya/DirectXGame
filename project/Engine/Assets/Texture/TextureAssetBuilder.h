#pragma once

#include "../BaseAssetBuilder.h"

#include <d3d12.h>
#include <wrl/client.h>

#include <memory>

class Texture;

class TextureAssetBuilder final : public BaseAssetBuilder {
public:
	TextureAssetBuilder(const std::filesystem::path& filePath);
	~TextureAssetBuilder() = default;

public:
	void preprocess() override;

	bool run() override;

	void postprocess() override;

	void transfer() override;

private:
	std::shared_ptr<Texture> textureData; // 実データ
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource; // 一時リソース
};
