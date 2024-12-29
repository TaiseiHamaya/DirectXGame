#include "DSVDescriptorHeap.h"

#include "Engine/Debug/Output.h"
#include "Engine/Rendering/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/Rendering/DirectX/DirectXSystemValues.h"

DSVDescriptorHeap& DSVDescriptorHeap::GetInstance() noexcept {
	static DSVDescriptorHeap instance{};
	return instance;
}

void DSVDescriptorHeap::Initialize() {
	GetInstance().create_descriptor_heap();
	GetInstance().initialize();
}

const std::uint32_t DSVDescriptorHeap::UseHeapIndex() noexcept {
	auto useIndex = GetInstance().use_heap_index();
	Console("Use DSV index. Index-\'{}\'\n", useIndex);
	return useIndex;
}

const D3D12_CPU_DESCRIPTOR_HANDLE DSVDescriptorHeap::GetCPUHandle(std::uint32_t index) noexcept {
	return GetInstance().get_cpu_handle(index);
}

void DSVDescriptorHeap::ReleaseHeapIndex(std::uint32_t index) {
	Console("Release DSV index. Index-\'{}\'\n", index);
	GetInstance().release_heap(index);
}

void DSVDescriptorHeap::create_descriptor_heap() {
	descriptorHeap = DirectXDescriptorHeap::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, DirectXSystemValues::HEAP_SIZE_DSV, false);
}

void DSVDescriptorHeap::initialize() {
	heapStartCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	// DSVはGPUHandle参照不可
	//heapStartGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	incrementSize = DirectXDevice::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	releasedHeap.clear();
	nowHeapIndex = 0;
}
