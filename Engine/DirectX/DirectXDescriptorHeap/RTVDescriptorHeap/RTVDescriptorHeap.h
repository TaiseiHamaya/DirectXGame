#pragma once

#include <dxgi1_6.h>
#include <array>
#include "Engine/DirectX/DirectXDescriptorHeap/DirectXDescriptorHeap.h"

class RTVDescriptorHeap final : public DirectXDescriptorHeap {
private:
	RTVDescriptorHeap() = default;

public:
	~RTVDescriptorHeap() = default;

private:
	RTVDescriptorHeap(const RTVDescriptorHeap&) = delete;
	RTVDescriptorHeap& operator=(const RTVDescriptorHeap&) = delete;

private:
	static RTVDescriptorHeap& GetInstance();

public:
	static void Initialize();
	static D3D12_CPU_DESCRIPTOR_HANDLE GetNextCPUHandle();

private:
	void create_descriptor_heap() override;
	void initialize();

	D3D12_GPU_DESCRIPTOR_HANDLE get_gpu_handle(uint32_t index) const = delete;
};
