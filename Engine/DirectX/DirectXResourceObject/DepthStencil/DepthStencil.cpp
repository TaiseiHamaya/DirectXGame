#include "DepthStencil.h"

#include <cassert>

#include "Engine/WinApp.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/DirectX/DirectXDescriptorHeap/DSVDescroptorHeap/DSVDescriptorHeap.h"

void DepthStencil::initialize() {
	resource = create_depth_stencil_texture_resource();
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	descriptorHandleCPU = DSVDescriptorHeap::GetNextCPUHandle();
	DirectXDevice::GetDevice()->CreateDepthStencilView(
		resource.Get(), 
		&dsvDesc, 
		descriptorHandleCPU
	);
	// DepthStencilの設定
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
}

const D3D12_CPU_DESCRIPTOR_HANDLE& DepthStencil::get_cpu_handle() const {
	return descriptorHandleCPU;
}

const D3D12_DEPTH_STENCIL_DESC& DepthStencil::get_depth_stencil_desc() const {
	return depthStencilDesc;
}

Microsoft::WRL::ComPtr<ID3D12Resource> DepthStencil::create_depth_stencil_texture_resource() {
	HRESULT hr;
	Microsoft::WRL::ComPtr<ID3D12Resource> resource; // 戻り値に使う
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = WinApp::GetClientWidth();
	resourceDesc.Height = WinApp::GetClientHight();
	resourceDesc.MipLevels = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f; // 1.0fで初期化
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	hr = DirectXDevice::GetDevice()->CreateCommittedResource(
		&heapProperties, 
		D3D12_HEAP_FLAG_NONE, 
		&resourceDesc, 
		D3D12_RESOURCE_STATE_DEPTH_WRITE, 
		&depthClearValue, 
		IID_PPV_ARGS(&resource)
	);
	assert(SUCCEEDED(hr));
	return resource;
}
