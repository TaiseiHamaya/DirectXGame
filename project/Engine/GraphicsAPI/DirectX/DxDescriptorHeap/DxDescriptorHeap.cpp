#include "DxDescriptorHeap.h"

#include <cassert>

#include "Engine/GraphicsAPI/DirectX/DxDevice/DxDevice.h"

const D3D12_CPU_DESCRIPTOR_HANDLE DxDescriptorHeap::get_cpu_handle(uint32_t index) const noexcept {
	D3D12_CPU_DESCRIPTOR_HANDLE result = heapStartCPU; // スタートから
	result.ptr += static_cast<SIZE_T>(incrementSize * index); // increment * index分ポインタを進める
	return result;
}

const D3D12_GPU_DESCRIPTOR_HANDLE DxDescriptorHeap::get_gpu_handle(std::uint32_t index) const noexcept {
	D3D12_GPU_DESCRIPTOR_HANDLE result = heapStartGPU; // 上に同じ
	result.ptr += incrementSize * static_cast<UINT64>(index);
	return result;
}

const std::uint32_t DxDescriptorHeap::use_heap_index() noexcept {
	if (releasedHeap.empty()) {
		return nowHeapIndex++;
	}
	else {
		std::uint32_t next = *releasedHeap.begin();
		releasedHeap.erase(releasedHeap.begin());
		return next;
	}
}

void DxDescriptorHeap::release_heap(std::uint32_t index) {
	releasedHeap.emplace(index);
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DxDescriptorHeap::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT heapSize, bool shaderVisible) {
	// カウンターを正常に保つためにComPtrを使用
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap;
	// 引数を元に設定
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = heapSize;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr;
	// ここで生成
	hr = DxDevice::GetDevice()->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(descriptorHeap.GetAddressOf()));
	assert(SUCCEEDED(hr));
	return descriptorHeap;
}