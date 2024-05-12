#pragma once

#include "Engine/DirectX/DirectXDescriptorHeap/DirectXDescriptorHeap.h"

class SRVDescriptorHeap : public DirectXDescriptorHeap {
private:
	SRVDescriptorHeap() = default;

public:
	~SRVDescriptorHeap() = default;

private:
	SRVDescriptorHeap(const SRVDescriptorHeap&) = delete;
	SRVDescriptorHeap& operator=(const SRVDescriptorHeap&) = delete;

private:
	static SRVDescriptorHeap& GetInstance();

public:
	static void Initialize();
	static const std::uint32_t GetNextHandleIndex();
	static const D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(std::uint32_t index);
	static const D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(std::uint32_t index);
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& GetDescriptorHeap();

	static void SetDescriptorHeaps();

private:
	void create_descriptor_heap() override;
	void initialize();
};
