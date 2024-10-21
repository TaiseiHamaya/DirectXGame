#pragma once

#include "../DirectXResourceObject.h"
#include "Engine/DirectX/DirectXDescriptorHeap/SRVDescriptorHeap/SRVDescriptorHeap.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"

#include <span>
#include <type_traits>
#include <optional>

template<typename T>
concept StructuredBufferType =
std::same_as<T, std::remove_cvref_t<T>> && // CV修飾、Reference修飾されていない
// クラス、算術型
(std::is_class_v<T> || std::is_arithmetic_v<T>);


template<StructuredBufferType T>
class StructuredBuffer : public  DirectXResourceObject {
public:
	void initialize(uint32_t arraySize_);
	void finalize();

public:
	std::span<T>& get_array();
	const std::span<T>& get_carray() const;
	const D3D12_GPU_DESCRIPTOR_HANDLE& get_handle_gpu() const { return gpuHandle; };

private:
	void create_resource();
	void create_srv();
	void release_index();
	void map();
	void unmap();

private:
	T* data;
	uint32_t arraySize;
	std::span<T> span;

	std::optional<std::uint32_t> heapIndex;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle{};
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};
};

template<StructuredBufferType T>
inline void StructuredBuffer<T>::initialize(uint32_t arraySize_) {
	release_index();

	arraySize = arraySize_;
	create_resource();
	create_srv();
	map();
}

template<StructuredBufferType T>
inline void StructuredBuffer<T>::finalize() {
	unmap();
	release_index();
}

template<StructuredBufferType T>
inline std::span<T>& StructuredBuffer<T>::get_array() {
	return span;
}

template<StructuredBufferType T>
inline const std::span<T>& StructuredBuffer<T>::get_carray() const {
	return span;
}

template<StructuredBufferType T>
inline void StructuredBuffer<T>::create_resource() {
	resource = CreateBufferResource(arraySize * sizeof(T));
}

template<StructuredBufferType T>
inline void StructuredBuffer<T>::create_srv() {
	heapIndex = SRVDescriptorHeap::UseHeapIndex();
	cpuHandle = SRVDescriptorHeap::GetCPUHandle(heapIndex.value());
	gpuHandle = SRVDescriptorHeap::GetGPUHandle(heapIndex.value());

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = arraySize;
	srvDesc.Buffer.StructureByteStride = sizeof(T);

	DirectXDevice::GetDevice()->CreateShaderResourceView(resource.Get(), &srvDesc, cpuHandle);
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
	data = nullptr;
	resource->Unmap(0, nullptr);
}
