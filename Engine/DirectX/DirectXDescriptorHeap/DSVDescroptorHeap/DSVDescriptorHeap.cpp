#include "DSVDescriptorHeap.h"

#include <cassert>
#include <memory>

#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"

// ダブルバッファのみで使用するため2
constexpr uint32_t DSV_HEAP_SIZE = 1;

DSVDescriptorHeap& DSVDescriptorHeap::GetInstance() {
	static std::unique_ptr<DSVDescriptorHeap> instance{ new DSVDescriptorHeap };
	return *instance;
}

void DSVDescriptorHeap::Initialize() {
	GetInstance().create_descriptor_heap();
	GetInstance().initialize();
}

D3D12_CPU_DESCRIPTOR_HANDLE DSVDescriptorHeap::GetNextCPUHandle() {
	return GetInstance().get_cpu_handle(GetInstance().get_next_heap_index());
}

void DSVDescriptorHeap::create_descriptor_heap() {
	descriptorHeap = DirectXDescriptorHeap::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, DSV_HEAP_SIZE, false);
}

void DSVDescriptorHeap::initialize() {
	heapStartCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	// RTVはGPUHandle参照不可
	//heapStartGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	incrementSize = DirectXDevice::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	releasedHeap.clear();
	nowHeapIndex = 0;
}
