#pragma once

#include <d3d12.h>
#include <wrl/client.h>

#include <list>

class DxDescriptorHeap {
protected:
	DxDescriptorHeap() noexcept = default;

public:
	virtual ~DxDescriptorHeap() noexcept = default;

protected:
	DxDescriptorHeap(const DxDescriptorHeap&) = delete;
	DxDescriptorHeap& operator=(const DxDescriptorHeap&) = delete;

protected:
	virtual void create_descriptor_heap() = 0;
	const D3D12_CPU_DESCRIPTOR_HANDLE get_cpu_handle(u32 index) const noexcept;
	const D3D12_GPU_DESCRIPTOR_HANDLE get_gpu_handle(u32 index) const noexcept;
	const u32 use_heap_index() noexcept;
	void release_heap(u32 index);

protected:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE heapStartCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE heapStartGPU;
	std::list<u32> releasedHeap;
	u32 nowHeapIndex;
	UINT incrementSize;

protected: // Heap共有関数
	[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE heapType,
		UINT heapSize,
		bool shaderVisible
	);
};