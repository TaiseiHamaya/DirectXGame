#pragma once

#include "Engine/GraphicsAPI/DirectX/DxDescriptorHeap/DxDescriptorHeap.h"

class RTVDescriptorHeap final : public DxDescriptorHeap {
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
	static u32 UseHeapIndex() noexcept;
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(u32 index) noexcept;
	static void ReleaseIndex(u32 index);

private:
	void create_descriptor_heap() override;
	void initialize();

	D3D12_GPU_DESCRIPTOR_HANDLE get_gpu_handle(u32 index) const noexcept = delete;
};
