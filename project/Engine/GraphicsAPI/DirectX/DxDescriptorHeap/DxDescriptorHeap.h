#pragma once

#include <d3d12.h>
#include <set>
#include <wrl/client.h>

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
	const D3D12_CPU_DESCRIPTOR_HANDLE get_cpu_handle(std::uint32_t index) const noexcept;
	const D3D12_GPU_DESCRIPTOR_HANDLE get_gpu_handle(std::uint32_t index) const noexcept;
	const std::uint32_t use_heap_index() noexcept;
	void release_heap(std::uint32_t index);

protected:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE heapStartCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE heapStartGPU;
	std::set<std::uint32_t> releasedHeap;
	std::uint32_t nowHeapIndex;
	UINT incrementSize;

protected: // Heap共有関数
	[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE heapType,
		UINT heapSize,
		bool shaderVisible
	);
};