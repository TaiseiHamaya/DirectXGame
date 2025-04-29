#pragma once

#include <format>
#include <optional>
#include <span>
#include <typeinfo>

#include <Library/Utility/Tools/ConvertString.h>

#include "../ConceptCPUBuffer.h"
#include "../DxResource.h"
#include "Engine/GraphicsAPI/DirectX/DxDescriptorHeap/SRVDescriptorHeap/SRVDescriptorHeap.h"
#include "Engine/GraphicsAPI/DirectX/DxDevice/DxDevice.h"

template<ConceptCPUBufferACE T>
class StructuredBuffer : public  DxResource {
public:
	StructuredBuffer() noexcept = default;
	virtual ~StructuredBuffer() noexcept;

public:
	StructuredBuffer(const StructuredBuffer&) = delete;
	StructuredBuffer& operator=(const StructuredBuffer&) = delete;
	StructuredBuffer(StructuredBuffer&&) noexcept = default;
	StructuredBuffer& operator=(StructuredBuffer&&) noexcept = default;

public:
	void initialize(u32 arraySize_);

public:
	T& operator[](u32 i);
	const T& operator[](u32 i) const;
	const u32& size() const { return arraySize; }
	const D3D12_GPU_DESCRIPTOR_HANDLE& get_handle_gpu() const { return gpuHandle; };

private:
	void create_resource();
	void create_srv();
	void release_index();
	void map();
	void unmap();

private:
	T* data{ nullptr };
	u32 arraySize{ 0 };
	std::span<T> span;

	std::optional<u32> heapIndex;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};
};

template<ConceptCPUBufferACE T>
inline StructuredBuffer<T>::~StructuredBuffer() noexcept {
	unmap();
	release_index();
}

template<ConceptCPUBufferACE T>
inline void StructuredBuffer<T>::initialize(u32 arraySize_) {
	release_index();

	arraySize = arraySize_;
	create_resource();
	create_srv();
	map();
}

template<ConceptCPUBufferACE T>
inline T& StructuredBuffer<T>::operator[](u32 i) {
	return span[i];
}

template<ConceptCPUBufferACE T>
inline const T& StructuredBuffer<T>::operator[](u32 i) const {
	return span[i];
}

template<ConceptCPUBufferACE T>
inline void StructuredBuffer<T>::create_resource() {
	resource = CreateBufferResource(sizeof(T) * arraySize);
	std::wstring typeName = ConvertString(typeid(T).name());
	if constexpr (std::is_class_v<T> || std::is_enum_v<T>) {
		typeName = (std::find(typeName.begin(), typeName.end(), ' ') + 1)._Ptr;
	}
	resource->SetName(std::format(L"StructuredBuffer-{}", typeName).c_str());
}

template<ConceptCPUBufferACE T>
inline void StructuredBuffer<T>::create_srv() {
	heapIndex = SRVDescriptorHeap::UseHeapIndex();
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = SRVDescriptorHeap::GetCPUHandle(heapIndex.value());
	gpuHandle = SRVDescriptorHeap::GetGPUHandle(heapIndex.value());

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = arraySize;
	srvDesc.Buffer.StructureByteStride = sizeof(T);

	DxDevice::GetDevice()->CreateShaderResourceView(resource.Get(), &srvDesc, cpuHandle);
}

template<ConceptCPUBufferACE T>
inline void StructuredBuffer<T>::release_index() {
	if (heapIndex.has_value()) {
		SRVDescriptorHeap::ReleaseHeapIndex(heapIndex.value());
	}
}

template<ConceptCPUBufferACE T>
inline void StructuredBuffer<T>::map() {
	resource->Map(0, nullptr, reinterpret_cast<void**>(&data));
	span = std::span<T>{ data, arraySize };
}

template<ConceptCPUBufferACE T>
inline void StructuredBuffer<T>::unmap() {
	span = std::span<T, 0>();
	if (data) {
		resource->Unmap(0, nullptr);
	}
	data = nullptr;
}
