#pragma once

#include <dxgi1_6.h>
#include "Engine/DirectX/DirectXDescriptorHeap/DirectXDescriptorHeap.h"

class RTVDescriptorHeap final : public DirectXDescriptorHeap {
private:
	RTVDescriptorHeap() = default;

public:
	~RTVDescriptorHeap() = default;

private:
	RTVDescriptorHeap(const RTVDescriptorHeap&) = delete;
	RTVDescriptorHeap& operator=(const RTVDescriptorHeap&) = delete;

public:
	static void Initialize();
	static void SetSwapChain(IDXGISwapChain4* const swapChain);
	static const Descriptor& GetDescriptor(uint32_t index);
	static void SetRenderTarget();

private:
	void create_descriptor_heap() override;
	void initialize();
	void set_swap_chain(IDXGISwapChain4* const swapChain);

	D3D12_GPU_DESCRIPTOR_HANDLE get_gpu_handle(uint32_t index) const = delete;

private:
	static RTVDescriptorHeap& GetInstance();
};
