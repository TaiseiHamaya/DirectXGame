#include "DirectXDescriptorHeap.h"

#include <assert.h>

#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"

HRESULT hr;

Descriptor::Descriptor(Descriptor&& rhs) noexcept {
	resource = *rhs.resource.ReleaseAndGetAddressOf();
	heapHandleCPU = std::move(rhs.heapHandleCPU);
	heapHandleGPU = std::move(rhs.heapHandleGPU);
}

Descriptor& Descriptor::operator=(Descriptor&& rhs) noexcept {
	resource = *rhs.resource.ReleaseAndGetAddressOf();
	heapHandleCPU = std::move(rhs.heapHandleCPU);
	heapHandleGPU = std::move(rhs.heapHandleGPU);
	return *this;
}


D3D12_CPU_DESCRIPTOR_HANDLE DirectXDescriptorHeap::get_cpu_handle(uint32_t index) const {
	D3D12_CPU_DESCRIPTOR_HANDLE result = heapStartCPU;
	result.ptr += incrementSize * index;
	return result;
}

D3D12_GPU_DESCRIPTOR_HANDLE DirectXDescriptorHeap::get_gpu_handle(uint32_t index) const {
	D3D12_GPU_DESCRIPTOR_HANDLE result = heapStartGPU;
	result.ptr += incrementSize * index;
	return result;
}

const Descriptor& DirectXDescriptorHeap::get_descriptor(uint32_t index) const {
	return descriptors[index];
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DirectXDescriptorHeap::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT heapSize, bool shaderVisible) {
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = heapSize;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr = DirectXDevice::GetDevice()->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(descriptorHeap.GetAddressOf()));
	assert(SUCCEEDED(hr));
	return descriptorHeap;
}