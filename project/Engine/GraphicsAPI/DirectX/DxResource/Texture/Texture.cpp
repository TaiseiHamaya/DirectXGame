#include "Texture.h"

#include <comdef.h>
#include <d3dx12.h>

#include <format>

#include <Library/Utility/Tools/ConvertString.h>

#include "Engine/Debug/Output.h"
#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxDescriptorHeap/SRVDescriptorHeap/SRVDescriptorHeap.h"
#include "Engine/GraphicsAPI/DirectX/DxDevice/DxDevice.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/DxResource.h"

Texture::Texture() noexcept = default;

Texture::~Texture() noexcept = default;

void Texture::release_srv_heap() {
	if (heapIndex.has_value()) {
		SRVDescriptorHeap::ReleaseHeapIndex(heapIndex.value());
	}
}

const D3D12_GPU_DESCRIPTOR_HANDLE& Texture::get_gpu_handle() const {
	return gpuHandle;
}

const std::uint32_t& Texture::get_texture_width() const noexcept {
	return width;
}

const std::uint32_t& Texture::get_texture_height() const noexcept {
	return height;
}

Microsoft::WRL::ComPtr<ID3D12Resource> Texture::load(const std::filesystem::path& filePath) {
	Console("Start load texture. file-\'{}\'\n", filePath.string());
	auto loadData = LoadTextureData(filePath); // ロード
	if (loadData.index() == 0) {
		Console(L"Failed loading texture. Message-\'{}\'\n", _com_error(std::get<0>(loadData)).ErrorMessage());
		return nullptr;
	}
	DirectX::ScratchImage& mipImages = std::get<1>(loadData);
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	width = static_cast<std::uint32_t>(metadata.width);
	height = static_cast<std::uint32_t>(metadata.height);
	create_texture_resource(metadata); // リソース生成
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = upload_texture_data(mipImages); // Texに転送

	// リソースの設定をmetadataから読み込む
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);
	Console("Succeeded create intermediate.\n");
	return intermediateResource;
}

void Texture::create_resource_view() {
	Console("Create texture resource view.\n");
	// 使用するディスクリプタヒープを取得
	heapIndex = SRVDescriptorHeap::UseHeapIndex();
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = SRVDescriptorHeap::GetCPUHandle(heapIndex.value());
	gpuHandle = SRVDescriptorHeap::GetGPUHandle(heapIndex.value());
	// textureResourceに転送
	DxDevice::GetDevice()->CreateShaderResourceView(resource.Get(), &srvDesc, textureSrvHandleCPU);
	Console("Succeeded.\n");
}

void Texture::set_name(const std::string& fileName) {
	resource->SetName(ConvertString(std::format("Texture-SRV{}({})", heapIndex.value(), fileName)).c_str());
}

void Texture::create_texture_resource(const DirectX::TexMetadata& metadata) {
	HRESULT hr;
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
	assert(SUCCEEDED(hr));
}

Microsoft::WRL::ComPtr<ID3D12Resource> Texture::upload_texture_data(const DirectX::ScratchImage& mipImages) {
	std::vector<D3D12_SUBRESOURCE_DATA> subResources;
	DirectX::PrepareUpload(DxDevice::GetDevice().Get(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subResources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(resource.Get(), 0, UINT(subResources.size()));
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = DxResource::CreateBufferResource(intermediateSize);
	DxCommand::SetTextureCommand(resource, intermediateResource, subResources);
	return intermediateResource;
}

std::variant<HRESULT, DirectX::ScratchImage> Texture::LoadTextureData(const std::filesystem::path& filePath) {
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