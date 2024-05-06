#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <stdint.h>
#include <set>
#include <vector>

class DirectXDescriptorHeap {
protected:
	DirectXDescriptorHeap() = default;

public:
	virtual ~DirectXDescriptorHeap() = default;

protected:
	DirectXDescriptorHeap(const DirectXDescriptorHeap&) = delete;
	DirectXDescriptorHeap& operator=(const DirectXDescriptorHeap&) = delete;

protected:
	virtual void create_descriptor_heap() = 0;
	D3D12_CPU_DESCRIPTOR_HANDLE get_cpu_handle(uint32_t index) const;
	D3D12_GPU_DESCRIPTOR_HANDLE get_gpu_handle(uint32_t index) const;
	std::uint32_t get_next_heap_index();

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