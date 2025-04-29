#include "TextureAssetBuilder.h"

#include <comdef.h>
#include <d3dx12.h>

#include "./TextureAsset.h"
#include "./TextureLibrary.h"
#include "Engine/Application/Output.h"
#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxDevice/DxDevice.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/DxResource.h"

TextureAssetBuilder::TextureAssetBuilder(const std::filesystem::path& filePath_) {
	filePath = filePath_;
}

void TextureAssetBuilder::preprocess() {
	textureData = std::make_unique<TextureAsset>();
}

bool TextureAssetBuilder::run() {
	Infomation("Start load texture. file-\'{}\'", filePath.string());
	auto loadData = LoadTextureData(filePath); // ロード
	// 失敗時
	if (loadData.index() == 0) {
		Error(L"Failed loading texture. File-'{}', Message-\'{}\'", filePath.wstring(), _com_error(std::get<0>(loadData)).ErrorMessage());
		return false;
	}
	DirectX::ScratchImage& mipImages = std::get<1>(loadData);
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	// リソース生成
	resource = CreateResource(metadata);
	// 一時リソースを作成
	std::vector<D3D12_SUBRESOURCE_DATA> subResources;
	DirectX::PrepareUpload(DxDevice::GetDevice().Get(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subResources);
	// 転送用リソースの作成
	size_t intermediateSize = GetRequiredIntermediateSize(resource.Get(), 0, UINT(subResources.size()));
	intermediateResource = DxResource::CreateBufferResource(intermediateSize);

	DxCommand::SetTextureCommand(resource, intermediateResource, subResources);

	return true;
}

void TextureAssetBuilder::postprocess() {
	textureData->initialize(resource);
#ifdef DEBUG_FEATURES_ENABLE
	textureData->set_name(filePath.filename().native());
#endif // DEBUG_FEATURES_ENABLE
}

void TextureAssetBuilder::transfer() {
	TextureLibrary::Transfer(filePath.filename().string(), textureData);
}

std::variant<HRESULT, DirectX::ScratchImage> TextureAssetBuilder::LoadTextureData(const std::filesystem::path& filePath) {
	HRESULT hr;
	DirectX::ScratchImage image{};
	hr = DirectX::LoadFromWICFile(filePath.wstring().c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image); // ロード
	if (FAILED(hr)) {
		return hr;
	}

	DirectX::ScratchImage mipImage{};
	// Mipmapに変換
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImage);
	if (FAILED(hr)) {
		return hr;
	}

	return mipImage;
}

Microsoft::WRL::ComPtr<ID3D12Resource> TextureAssetBuilder::CreateResource(const DirectX::TexMetadata& metadata) {
	HRESULT hr;
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
	D3D12_HEAP_PROPERTIES heapProperties{};// 利用するHeapを設定
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // 通常設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension); // テクスチャの次元数
	resourceDesc.Width = UINT64(metadata.width); // 幅
	resourceDesc.Height = UINT(metadata.height); // 高さ
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize); // 奥行きor配列Textureの要素数
	resourceDesc.MipLevels = UINT16(metadata.mipLevels); // // mipmapの数
	resourceDesc.SampleDesc.Count = 1; // サンプリングカウント
	resourceDesc.Format = metadata.format; // Textureのフォーマット

	hr = DxDevice::GetDevice()->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(resource.GetAddressOf()));
	ErrorIf(FAILED(hr), "Failed to allocate GPU memory. Width-\'{}\', Height-\'{}\', Depth-\'{}\', MipLevel-\'{}\', Format-\'{}\'",
		metadata.width, metadata.height, metadata.arraySize, metadata.mipLevels, (i32)metadata.format);

	return resource;
}
