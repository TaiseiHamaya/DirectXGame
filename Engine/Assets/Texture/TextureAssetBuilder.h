#pragma once

#include "../BaseAssetBuilder.h"

#include <filesystem>
#include <memory>
#include <variant>

#include <d3d12.h>
#include <DirectXTex.h>
#include <wrl/client.h>

class TextureAsset;

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
	[[nodiscard]] static std::variant<HRESULT, DirectX::ScratchImage> LoadTextureData(const std::filesystem::path& filePath);
	[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D12Resource> CreateResource(const DirectX::TexMetadata& metadata);

private:
	std::shared_ptr<TextureAsset> textureData; // 実データ
	Microsoft::WRL::ComPtr<ID3D12Resource> resource; // リソースデータ
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource; // 一時リソース

	bool isCubemap{ false };
};
