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
	static std::uint32_t UseHeapIndex() noexcept;
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(std::uint32_t index) noexcept;
	static void ReleaseIndex(std::uint32_t index);

private:
	void create_descriptor_heap() override;
	void initialize();

	D3D12_GPU_DESCRIPTOR_HANDLE get_gpu_handle(std::uint32_t index) const noexcept = delete;
};
