#include "RTVDescriptorHeap.h"

#include "Engine/Debug/Output.h"
#include "Engine/Rendering/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/Rendering/DirectX/DirectXSystemValues.h"

RTVDescriptorHeap& RTVDescriptorHeap::GetInstance() noexcept {
	static RTVDescriptorHeap instance{};
	return instance;
}

void RTVDescriptorHeap::Initialize() {
	GetInstance().create_descriptor_heap();
	GetInstance().initialize();
}

std::uint32_t RTVDescriptorHeap::UseHeapIndex() noexcept {
	auto useIndex = GetInstance().use_heap_index();
	Console("Use RTV index. Index-\'{}\'\n", useIndex);
	return useIndex;
}

D3D12_CPU_DESCRIPTOR_HANDLE RTVDescriptorHeap::GetCPUHandle(std::uint32_t index) noexcept {
	return GetInstance().get_cpu_handle(index);
}

void RTVDescriptorHeap::ReleaseIndex(std::uint32_t index) {
	Console("Release RTV index. Index-\'{}\'\n", index);
	GetInstance().release_heap(index);
}

void RTVDescriptorHeap::create_descriptor_heap() {
	descriptorHeap = DirectXDescriptorHeap::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, DirectXSystemValues::HEAP_SIZE_RTV, false);
}

void RTVDescriptorHeap::initialize() {
	heapStartCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	// RTVはGPUHandle参照不可
	//heapStartGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	incrementSize = DirectXDevice::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	releasedHeap.clear();
	nowHeapIndex = 0;
}
