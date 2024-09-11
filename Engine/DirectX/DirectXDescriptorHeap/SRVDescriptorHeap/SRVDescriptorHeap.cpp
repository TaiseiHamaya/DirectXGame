#include "SRVDescriptorHeap.h"

#include <memory>
#include <format>

#include "Engine/Utility/Utility.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"

// SRVのHeapは65536
constexpr uint32_t SRV_HEAP_SIZE = 65536;

SRVDescriptorHeap& SRVDescriptorHeap::GetInstance() noexcept {
	static std::unique_ptr<SRVDescriptorHeap> instance{ new SRVDescriptorHeap };
	return *instance;
}

void SRVDescriptorHeap::Initialize() {
	GetInstance().create_descriptor_heap();
	GetInstance().initialize();
}

const std::uint32_t SRVDescriptorHeap::UseHeapIndex() noexcept {
	auto useIndex = GetInstance().use_heap_index();
	Log(std::format("[SRVDescriptorHeap] Use SRV index. Index-\'{}\'\n", useIndex));
	return useIndex;
}

const D3D12_CPU_DESCRIPTOR_HANDLE SRVDescriptorHeap::GetCPUHandle(std::uint32_t index) noexcept {
	return GetInstance().get_cpu_handle(index);
}

const D3D12_GPU_DESCRIPTOR_HANDLE SRVDescriptorHeap::GetGPUHandle(std::uint32_t index) noexcept {
	return GetInstance().get_gpu_handle(index);
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& SRVDescriptorHeap::GetDescriptorHeap() noexcept {
	return GetInstance().descriptorHeap;
}

void SRVDescriptorHeap::SetDescriptorHeaps() {
	ID3D12DescriptorHeap* descriptorHeaps[] = { GetInstance().descriptorHeap.Get() };
	DirectXCommand::GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps); // SRVDescriptorHeap
}

void SRVDescriptorHeap::ReleaseHeapIndex(std::uint32_t index) {
	Log(std::format("[SRVDescriptorHeap] Release SRV index. Index-\'{}\'\n", index));
	GetInstance().release_heap(index);
}

void SRVDescriptorHeap::create_descriptor_heap() {
	descriptorHeap = DirectXDescriptorHeap::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, SRV_HEAP_SIZE, true);
}

void SRVDescriptorHeap::initialize() {
	heapStartCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	heapStartGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	incrementSize = DirectXDevice::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	releasedHeap.clear();
	nowHeapIndex = 0;
}
