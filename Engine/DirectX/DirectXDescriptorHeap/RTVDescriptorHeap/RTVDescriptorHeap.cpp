#include "RTVDescriptorHeap.h"

#include <memory>
#include <format>

#include "Engine/Utility/Utility.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"

// ダブルバッファ+PEで3
constexpr uint32_t RTV_HEAP_SIZE = 10;

RTVDescriptorHeap& RTVDescriptorHeap::GetInstance() noexcept {
	static std::unique_ptr<RTVDescriptorHeap> instance{ new RTVDescriptorHeap };
	return *instance;
}

void RTVDescriptorHeap::Initialize() {
	GetInstance().create_descriptor_heap();
	GetInstance().initialize();
}

std::uint32_t RTVDescriptorHeap::UseHeapIndex() noexcept {
	Log(std::format("[RTVDescriptorHeap] Use RTV index. Index-\'{}\'\n", GetInstance().nowHeapIndex));
    return GetInstance().use_heap_index();
}

D3D12_CPU_DESCRIPTOR_HANDLE RTVDescriptorHeap::GetCPUHandle(std::uint32_t index) noexcept {
	return GetInstance().get_cpu_handle(index);
}

void RTVDescriptorHeap::ReleaseIndex(std::uint32_t index) {
	Log(std::format("[RTVDescriptorHeap] Release RTV index. Index-\'{}\'\n", index));
	GetInstance().release_heap(index);
}

void RTVDescriptorHeap::create_descriptor_heap() {
	descriptorHeap = DirectXDescriptorHeap::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, RTV_HEAP_SIZE, false);
}

void RTVDescriptorHeap::initialize() {
	heapStartCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	// RTVはGPUHandle参照不可
	//heapStartGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	incrementSize = DirectXDevice::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	releasedHeap.clear();
	nowHeapIndex = 0;
}
