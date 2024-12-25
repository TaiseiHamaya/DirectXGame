#pragma once

#include "../BaseResourceBuilder.h"

#include <d3d12.h>
#include <wrl/client.h>

#include <memory>

class Texture;

class TextureResourceBuilder final : public BaseResourceBuilder {
public:
	TextureResourceBuilder(const std::filesystem::path& filePath);
	~TextureResourceBuilder() = default;

public:
	void preprocess() override;

	bool run() override;

	void postprocess() override;

	void transfer() override;

private:
	std::shared_ptr<Texture> textureData; // 実データ
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource; // 一時リソース
};
