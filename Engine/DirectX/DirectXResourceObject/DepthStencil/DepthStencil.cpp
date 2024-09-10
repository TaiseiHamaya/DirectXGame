#include "DepthStencil.h"

#include <cassert>

#include "Engine/DirectX/DirectXDescriptorHeap/DSVDescroptorHeap/DSVDescriptorHeap.h"
#include "Engine/DirectX/DirectXDescriptorHeap/SRVDescriptorHeap/SRVDescriptorHeap.h"
#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/WinApp.h"

void DepthStencil::initialize(std::uint32_t width, std::uint32_t height) {
	create_depth_stencil_texture_resource(width, height);
	create_dsv();
	create_srv();
	// DepthStencilの設定
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	// Z値が{ 描画済みのピクセルの深度値 >= 描画予定のピクセルの深度値 }の場合描画
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	// 最初は描画していない状態
	isWriting = false;
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

void DepthStencil::change_resource_state() {
	DirectXCommand::SetBarrier(
		resource,
		isWriting ? D3D12_RESOURCE_STATE_DEPTH_WRITE : D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		isWriting ? D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE : D3D12_RESOURCE_STATE_DEPTH_WRITE
	);
	// 描画の状態を反転
	isWriting = isWriting ^ 0b1;
}

void DepthStencil::create_depth_stencil_texture_resource(std::uint32_t width, std::uint32_t height) {
	HRESULT hr;
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = WinApp::GetClientWidth();
	resourceDesc.Height = WinApp::GetClientHight();
	resourceDesc.MipLevels = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 深度に24ビット、ステンシルに8ビット
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 画面全体なので2D
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // DSとして使用

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f; // 1.0fで初期化
	depthClearValue.Format = resourceDesc.Format;

	hr = DirectXDevice::GetDevice()->CreateCommittedResource(
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
	dsvCPUHandle = DSVDescriptorHeap::UseNextHandle();
	// viewの作成
	DirectXDevice::GetDevice()->CreateDepthStencilView(
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
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS; // DSの場合はこれで固定
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1; // 1枚しかないのでmiplevelsも1
	// ここも変わらない
	DirectXDevice::GetDevice()->CreateShaderResourceView(resource.Get(), &srvDesc, srvCPUHandle);


}
