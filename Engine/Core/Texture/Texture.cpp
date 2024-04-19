#include "Texture.h"

#include "Engine/D3D12Kernel.h"
#include "Engine/Util.h"
#include <d3dx12.h>

static HRESULT hr;

void Texture::set_command(ID3D12GraphicsCommandList* const commandList) {
	commandList->SetGraphicsRootDescriptorTable(heapIndex, gpuHandle); // Texture
}

void Texture::load_texture(const std::string& FilePath, uint32_t heapIndex) {
	DirectX::ScratchImage mipImages;
	mipImages = LoadTextureData(FilePath); // ロード
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	create_texture_resource(metadata); // リソース生成
	ID3D12Resource* intermadiateResource = upload_texture_data(mipImages); // Texに転送

	intermadiateResource->Release(); // これ以降使わないのでリリース

	// リソースの設定をmetadataから読み込む
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	// 使用するディスクリプタヒープを取得
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = D3D12Kernel::GetInstance()->GetCPUDescriptorHandleSRV(heapIndex);
	// textureResourceに転送
	D3D12Kernel::GetInstance()->GetDevice()->CreateShaderResourceView(textureResource, &srvDesc, textureSrvHandleCPU);
}

void Texture::create_texture_resource(const DirectX::TexMetadata& metadata) {
	D3D12_HEAP_PROPERTIES heapPropertis{};// 利用するHeapを設定
	heapPropertis.Type = D3D12_HEAP_TYPE_DEFAULT; // 通常設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension); // テクスチャの次元数
	resourceDesc.Width = UINT(metadata.width); // 幅
	resourceDesc.Height = UINT(metadata.height); // 高さ
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize); // 奥行きor配列Textureの要素数
	resourceDesc.MipLevels = UINT16(metadata.mipLevels); // // mipmapの数
	resourceDesc.SampleDesc.Count = 1; // サンプリングカウント
	resourceDesc.Format = metadata.format; // Textureのフォーマット

	hr = D3D12Kernel::GetInstance()->GetDevice()->CreateCommittedResource(&heapPropertis, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&textureResource));
	assert(SUCCEEDED(hr));
}

ID3D12Resource* Texture::upload_texture_data(const DirectX::ScratchImage& mipImages) {
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	DirectX::PrepareUpload(D3D12Kernel::GetInstance()->GetDevice(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(textureResource, 0, UINT(subresources.size()));
	ID3D12Resource* intermediateResource = D3D12Kernel::GetInstance()->create_buffer_resource(intermediateSize);
	UpdateSubresources(commandList, textureResource, intermediateResource, 0, 0, UINT(subresources.size()), subresources.data()); // コマンドに積む
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = textureResource;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	commandList->ResourceBarrier(1, &barrier); // textureのリソースバリアを変更
	return intermediateResource;
}

DirectX::ScratchImage Texture::LoadTextureData(const std::string& filePath) {
	DirectX::ScratchImage image{};
	auto&& filePathW = ConvertString(filePath); // Wcharに変換
	hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image); // ロード
	assert(SUCCEEDED(hr));

	DirectX::ScratchImage mipImage{};
	// Mipmapに変換
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImage);
	assert(SUCCEEDED(hr));

	return mipImage;
}