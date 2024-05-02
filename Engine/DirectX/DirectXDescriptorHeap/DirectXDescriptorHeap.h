#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <stdint.h>
#include <set>
#include <vector>

class Descriptor {
public:
	Descriptor() = default;
	~Descriptor() = default;

public:
	Descriptor(const Descriptor&) = delete;
	Descriptor& operator=(const Descriptor&) = delete;
	// ムーブは許可
	Descriptor(Descriptor&& rhs) noexcept;
	Descriptor& operator=(Descriptor&& rhs) noexcept;

public:
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE heapHandleGPU;
};

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
	const Descriptor& get_descriptor(uint32_t index) const;

protected:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap;
	std::vector<Descriptor> descriptors;
	D3D12_CPU_DESCRIPTOR_HANDLE heapStartCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE heapStartGPU;
	std::set<D3D12_CPU_DESCRIPTOR_HANDLE> releasedHeap;
	UINT incrementSize;

protected: // Heap共有関数
	[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE heapType,
		UINT heapSize,
		bool shaderVisible
	);
};