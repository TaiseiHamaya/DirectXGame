#include "DepthStencil.h"

#include <cassert>
#include <format>

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxDescriptorHeap/DSVDescroptorHeap/DSVDescriptorHeap.h"
#include "Engine/GraphicsAPI/DirectX/DxDescriptorHeap/SRVDescriptorHeap/SRVDescriptorHeap.h"
#include "Engine/GraphicsAPI/DirectX/DxDevice/DxDevice.h"

DepthStencil::~DepthStencil() {
	if (resource) {
		release_index();
	}
}

void DepthStencil::initialize(DXGI_FORMAT format, std::uint32_t width, std::uint32_t height) {
	create_depth_stencil_texture_resource(width, height, format);
	create_dsv();
	create_srv();
	resource->SetName(std::format(L"DepthStencil-DSV{}, SRV{}", dsvHeapIndex.value(), srvHeapIndex.value()).c_str());
	// DepthStencilの設定
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	// Z値が{ 描画済みのピクセルの深度値 > 描画予定のピクセルの深度値 }の場合描画
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	// 最初は描画していない状態
	isWriting = false;
}

void DepthStencil::change_resource_state() {
	DxCommand::SetBarrier(
		resource,
		isWriting ? D3D12_RESOURCE_STATE_DEPTH_WRITE : D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		isWriting ? D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE : D3D12_RESOURCE_STATE_DEPTH_WRITE
	);
	// 描画の状態を反転
	isWriting = isWriting ^ 0b1;
}

void DepthStencil::release_index() {
	if (dsvHeapIndex.has_value()) {
		DSVDescriptorHeap::ReleaseHeapIndex(dsvHeapIndex.value());
	}
	if (srvHeapIndex.has_value()) {
		SRVDescriptorHeap::ReleaseHeapIndex(srvHeapIndex.value());
	}
	dsvHeapIndex = std::nullopt;
	srvHeapIndex = std::nullopt;
}

const D3D12_CPU_DESCRIPTOR_HANDLE& DepthStencil::get_dsv_cpu_handle() const noexcept {
	return dsvCPUHandle;
}

const D3D12_DEPTH_STENCIL_DESC& DepthStencil::get_desc() const noexcept {
	return depthStencilDesc;
}

D3D12_GPU_DESCRIPTOR_HANDLE DepthStencil::texture_gpu_handle() const {
	return srvGPUHandle;
}

DXGI_FORMAT DepthStencil::texture_format() const {
	// DXGI_FORMATのD~の次の項目がTexture用として使用できるフォーマットになっている
	// こうすることで無理やりFormatを合わせることができる
	return static_cast<DXGI_FORMAT>(resource->GetDesc().Format + 1);
}

void DepthStencil::create_depth_stencil_texture_resource(std::uint32_t width, std::uint32_t height, DXGI_FORMAT format) {
	HRESULT hr;
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = width;
	resourceDesc.Height = height;
	resourceDesc.MipLevels = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Format = format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 画面全体なので2D
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // DSとして使用

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f; // 1.0fで初期化
	depthClearValue.Format = resourceDesc.Format;

	hr = DxDevice::GetDevice()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&depthClearValue,
		IID_PPV_ARGS(resource.GetAddressOf())
	);
	assert(SUCCEEDED(hr));
}

void DepthStencil::create_dsv() {
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = resource->GetDesc().Format;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; // 画面全体なので2D
	dsvHeapIndex = DSVDescriptorHeap::UseHeapIndex();
	dsvCPUHandle = DSVDescriptorHeap::GetCPUHandle(dsvHeapIndex.value());
	// viewの作成
	DxDevice::GetDevice()->CreateDepthStencilView(
		resource.Get(),
		&dsvDesc,
		dsvCPUHandle
	);
}

void DepthStencil::create_srv() {
	srvHeapIndex = SRVDescriptorHeap::UseHeapIndex();
	srvCPUHandle = SRVDescriptorHeap::GetCPUHandle(srvHeapIndex.value());
	srvGPUHandle = SRVDescriptorHeap::GetGPUHandle(srvHeapIndex.value());
	// ここは通常のテクスチャと同じ
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = texture_format();
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1; // 1枚しかないのでmiplevelsも1
	// ここも変わらない
	DxDevice::GetDevice()->CreateShaderResourceView(resource.Get(), &srvDesc, srvCPUHandle);
}
