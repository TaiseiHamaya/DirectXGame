#include "DirectXDescriptorHeap.h"

#include <assert.h>

#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"

HRESULT hr;

const D3D12_CPU_DESCRIPTOR_HANDLE DirectXDescriptorHeap::get_cpu_handle(uint32_t index) const {
	D3D12_CPU_DESCRIPTOR_HANDLE result = heapStartCPU; // スタートから
	result.ptr += incrementSize * index; // increment * index分ポインタを進める
	return result;
}

const D3D12_GPU_DESCRIPTOR_HANDLE DirectXDescriptorHeap::get_gpu_handle(uint32_t index) const {
	D3D12_GPU_DESCRIPTOR_HANDLE result = heapStartGPU; // 上に同じ
	result.ptr += incrementSize * index;
	return result;
}

const std::uint32_t DirectXDescriptorHeap::get_next_heap_index() {
	if (releasedHeap.empty()) {
		return nowHeapIndex++;
	}
	else {
		std::uint32_t next = *releasedHeap.begin();
		releasedHeap.erase(releasedHeap.begin());
		return next;
	}
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DirectXDescriptorHeap::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT heapSize, bool shaderVisible) {
	// カウンターを正常に保つためにComPtrを使用
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap;
	// 引数を元に設定
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = heapSize;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	// ここで生成
	hr = DirectXDevice::GetDevice()->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(descriptorHeap.GetAddressOf()));
	assert(SUCCEEDED(hr));
	return descriptorHeap;
}