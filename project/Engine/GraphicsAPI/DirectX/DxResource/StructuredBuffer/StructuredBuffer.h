#pragma once

#include "../DxResource.h"
#include "./StructuredConcept.h"
#include "Engine/GraphicsAPI/DirectX/DxDescriptorHeap/SRVDescriptorHeap/SRVDescriptorHeap.h"
#include "Engine/GraphicsAPI/DirectX/DxDevice/DxDevice.h"

#include <optional>
#include <span>

template<StructuredBufferType T>
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
	void initialize(uint32_t arraySize_);

public:
	T& operator[](uint32_t i);
	const T& operator[](uint32_t i) const;
	const uint32_t& size() const { return arraySize; }
	const D3D12_GPU_DESCRIPTOR_HANDLE& get_handle_gpu() const { return gpuHandle; };

private:
	void create_resource();
	void create_srv();
	void release_index();
	void map();
	void unmap();

private:
	T* data{ nullptr };
	uint32_t arraySize{ 0 };
	std::span<T> span;

	std::optional<uint32_t> heapIndex;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};
};

template<StructuredBufferType T>
inline StructuredBuffer<T>::~StructuredBuffer() noexcept {
	unmap();
	release_index();
}

template<StructuredBufferType T>
inline void StructuredBuffer<T>::initialize(uint32_t arraySize_) {
	release_index();

	arraySize = arraySize_;
	create_resource();
	create_srv();
	map();
}

template<StructuredBufferType T>
inline T& StructuredBuffer<T>::operator[](uint32_t i) {
	return span[i];
}

template<StructuredBufferType T>
inline const T& StructuredBuffer<T>::operator[](uint32_t i) const {
	return span[i];
}

template<StructuredBufferType T>
inline void StructuredBuffer<T>::create_resource() {
	resource = CreateBufferResource(sizeof(T) * arraySize);
}

template<StructuredBufferType T>
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

template<StructuredBufferType T>
inline void StructuredBuffer<T>::release_index() {
	if (heapIndex.has_value()) {
		SRVDescriptorHeap::ReleaseHeapIndex(heapIndex.value());
	}
}

template<StructuredBufferType T>
inline void StructuredBuffer<T>::map() {
	resource->Map(0, nullptr, reinterpret_cast<void**>(&data));
	span = std::span<T>{ data, arraySize };
}

template<StructuredBufferType T>
inline void StructuredBuffer<T>::unmap() {
	span = std::span<T, 0>();
	if (data) {
		resource->Unmap(0, nullptr);
	}
	data = nullptr;
}
