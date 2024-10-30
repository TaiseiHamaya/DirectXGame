#pragma once

#include "Engine/Rendering/DirectX/DirectXDescriptorHeap/DirectXDescriptorHeap.h"

class SRVDescriptorHeap final : public DirectXDescriptorHeap {
private:
	SRVDescriptorHeap() noexcept = default;

public:
	~SRVDescriptorHeap() noexcept = default;

private:
	SRVDescriptorHeap(const SRVDescriptorHeap&) = delete;
	SRVDescriptorHeap& operator=(const SRVDescriptorHeap&) = delete;

private:
	static SRVDescriptorHeap& GetInstance() noexcept;

public:
	static void Initialize();
	static const std::uint32_t UseHeapIndex() noexcept;
	static const D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(std::uint32_t index) noexcept;
	static const D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(std::uint32_t index) noexcept;
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& GetDescriptorHeap() noexcept;

	static void SetDescriptorHeaps();

	static void ReleaseHeapIndex(std::uint32_t index);

private:
	void create_descriptor_heap() override;
	void initialize();
};
