#pragma once

#include "Engine/DirectX/DirectXDescriptorHeap/DirectXDescriptorHeap.h"

class DSVDescriptorHeap : public DirectXDescriptorHeap {
private:
	DSVDescriptorHeap() = default;

public:
	~DSVDescriptorHeap() = default;

private:
	DSVDescriptorHeap(const DSVDescriptorHeap&) = delete;
	DSVDescriptorHeap& operator=(const DSVDescriptorHeap&) = delete;

private:
	static DSVDescriptorHeap& GetInstance();

public:
	static void Initialize();
	static D3D12_CPU_DESCRIPTOR_HANDLE GetNextCPUHandle();

private:
	void create_descriptor_heap() override;
	void initialize();

	D3D12_GPU_DESCRIPTOR_HANDLE get_gpu_handle(uint32_t index) const = delete;
};
