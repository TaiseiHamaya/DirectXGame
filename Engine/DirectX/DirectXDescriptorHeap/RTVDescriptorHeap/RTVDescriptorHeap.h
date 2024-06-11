#pragma once

#include "Engine/DirectX/DirectXDescriptorHeap/DirectXDescriptorHeap.h"

class RTVDescriptorHeap final : public DirectXDescriptorHeap {
private:
	RTVDescriptorHeap() noexcept = default;

public:
	~RTVDescriptorHeap() noexcept = default;

private:
	RTVDescriptorHeap(const RTVDescriptorHeap&) = delete;
	RTVDescriptorHeap& operator=(const RTVDescriptorHeap&) = delete;

private:
	static RTVDescriptorHeap& GetInstance() noexcept;

public:
	static void Initialize();
	static D3D12_CPU_DESCRIPTOR_HANDLE GetNextCPUHandle() noexcept;

private:
	void create_descriptor_heap() override;
	void initialize();

	D3D12_GPU_DESCRIPTOR_HANDLE get_gpu_handle(std::uint32_t index) const noexcept = delete;
};
