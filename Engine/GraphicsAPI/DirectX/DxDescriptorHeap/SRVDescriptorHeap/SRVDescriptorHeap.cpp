#include "SRVDescriptorHeap.h"

#include "Engine/Application/Output.h"
#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxDevice/DxDevice.h"
#include "Engine/GraphicsAPI/DirectX/DxSystemValues.h"

#include <mutex>

std::mutex srvHeapMutex;

SRVDescriptorHeap& SRVDescriptorHeap::GetInstance() noexcept {
	static SRVDescriptorHeap instance{};
	return instance;
}

void SRVDescriptorHeap::Initialize() {
	GetInstance().create_descriptor_heap();
	GetInstance().initialize();
}

const u32 SRVDescriptorHeap::UseHeapIndex() noexcept {
	std::lock_guard<std::mutex> lock{ srvHeapMutex };
	auto useIndex = GetInstance().use_heap_index();
	Information("Use SRV index. Index-\'{}\'", useIndex);
	return useIndex;
}

const D3D12_CPU_DESCRIPTOR_HANDLE SRVDescriptorHeap::GetCPUHandle(u32 index) noexcept {
	return GetInstance().get_cpu_handle(index);
}

const D3D12_GPU_DESCRIPTOR_HANDLE SRVDescriptorHeap::GetGPUHandle(u32 index) noexcept {
	return GetInstance().get_gpu_handle(index);
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& SRVDescriptorHeap::GetDescriptorHeap() noexcept {
	return GetInstance().descriptorHeap;
}

void SRVDescriptorHeap::SetDescriptorHeaps() {
	ID3D12DescriptorHeap* descriptorHeaps[] = { GetInstance().descriptorHeap.Get() };
	DxCommand::GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps); // SRVDescriptorHeap
}

void SRVDescriptorHeap::ReleaseHeapIndex(u32 index) {
	Information("Release SRV index. Index-\'{}\'", index);
	GetInstance().release_heap(index);
}

void SRVDescriptorHeap::create_descriptor_heap() {
	descriptorHeap = DxDescriptorHeap::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, DxSystemValues::HEAP_SIZE_SRV, true);
}

void SRVDescriptorHeap::initialize() {
	heapStartCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	heapStartGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	incrementSize = DxDevice::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	releasedHeap.clear();
	nowHeapIndex = 0;
}
