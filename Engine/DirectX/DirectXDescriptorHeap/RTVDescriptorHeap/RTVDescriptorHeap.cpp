#include "RTVDescriptorHeap.h"

#include <memory>
#include <cassert>

#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"

RTVDescriptorHeap& RTVDescriptorHeap::GetInstance() {
	static std::unique_ptr<RTVDescriptorHeap> instance{ new RTVDescriptorHeap };
	return *instance;
}

void RTVDescriptorHeap::Initialize() {
	GetInstance();
	GetInstance().create_descriptor_heap();
	GetInstance().initialize();
}

D3D12_CPU_DESCRIPTOR_HANDLE RTVDescriptorHeap::GetNextCPUHandle() {
	return GetInstance().get_cpu_handle(GetInstance().get_next_heap_index());
}

void RTVDescriptorHeap::create_descriptor_heap() {
	descriptorHeap = DirectXDescriptorHeap::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, HEAPSIZE, false);
}

void RTVDescriptorHeap::initialize() {
	heapStartCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	// RTVはGPUHandle参照不可
	//heapStartGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	incrementSize = DirectXDevice::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	releasedHeap.clear();
	nowHeapIndex = 0;
}
