#pragma once

#include "Engine/GraphicsAPI/DirectX/DxDescriptorHeap/DxDescriptorHeap.h"

class DSVDescriptorHeap final : public DxDescriptorHeap {
private:
	DSVDescriptorHeap() noexcept = default;

public:
	~DSVDescriptorHeap() noexcept = default;

private:
	DSVDescriptorHeap(const DSVDescriptorHeap&) = delete;
	DSVDescriptorHeap& operator=(const DSVDescriptorHeap&) = delete;

private:
	static DSVDescriptorHeap& GetInstance() noexcept;

public:
	static void Initialize();
	static const std::uint32_t UseHeapIndex() noexcept;
	static const D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(std::uint32_t index) noexcept;
	static void ReleaseHeapIndex(std::uint32_t index);

private:
	void create_descriptor_heap() override;
	void initialize();

	D3D12_GPU_DESCRIPTOR_HANDLE get_gpu_handle(std::uint32_t index) const noexcept = delete;
};
