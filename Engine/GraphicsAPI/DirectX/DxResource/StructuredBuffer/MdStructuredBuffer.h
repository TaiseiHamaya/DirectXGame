#pragma once

#include "../ConceptCPUBuffer.h"
#include "../DxResource.h"
#include "Engine/GraphicsAPI/DirectX/DxDescriptorHeap/SRVDescriptorHeap/SRVDescriptorHeap.h"
#include "Engine/GraphicsAPI/DirectX/DxDevice/DxDevice.h"

#include <optional>
#include <span>

#ifdef DEBUG_FEATURES_ENABLE
#include <stdexcept>
#endif // _DEBUG

template<ConceptCPUBufferACE T>
class MdStructuredBuffer : public  DxResource {
public:
	MdStructuredBuffer() noexcept = default;
	virtual ~MdStructuredBuffer() noexcept;

public:
	MdStructuredBuffer(const MdStructuredBuffer&) = delete;
	MdStructuredBuffer& operator=(const MdStructuredBuffer&) = delete;
	MdStructuredBuffer(MdStructuredBuffer&&) noexcept = default;
	MdStructuredBuffer& operator=(MdStructuredBuffer&&) noexcept = default;

public:
	void initialize(u32 numInstance, u32 arraySize_);

public:
	std::span<T> operator[](u32 i) const;
	const D3D12_GPU_DESCRIPTOR_HANDLE& get_handle_gpu() const { return gpuHandle; };

private:
	void create_resource();
	void create_srv();
	void release_index();
	void map();
	void unmap();

private:
	T* data{ nullptr };
	u32 numInstance{ 0 };
	u32 arraySize{ 0 };

	std::optional<u32> heapIndex;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};
};

template<ConceptCPUBufferACE T>
inline MdStructuredBuffer<T>::~MdStructuredBuffer() noexcept {
	unmap();
	release_index();
}

template<ConceptCPUBufferACE T>
inline void MdStructuredBuffer<T>::initialize(u32 numInstance_, u32 arraySize_) {
	if (numInstance * arraySize == numInstance_ * arraySize_) {
		arraySize = arraySize_;
		numInstance = numInstance_;
		return;
	}

	release_index();

	arraySize = arraySize_;
	numInstance = numInstance_;
	create_resource();
	create_srv();
	map();
}

template<ConceptCPUBufferACE T>
inline std::span<T> MdStructuredBuffer<T>::operator[](u32 i) const {
#ifdef DEBUG_FEATURES_ENABLE
	if (i >= numInstance) {
		throw std::out_of_range("index is out of range.");
	}
#endif // _DEBUG

	T* base = &data[i * arraySize];
	return { base, arraySize };
}

template<ConceptCPUBufferACE T>
inline void MdStructuredBuffer<T>::create_resource() {
	resource = CreateBufferResource(sizeof(T) * arraySize * numInstance);
	std::wstring typeName = ConvertString(typeid(T).name());
	if constexpr (std::is_class_v<T> || std::is_enum_v<T>) {
		typeName = (std::find(typeName.begin(), typeName.end(), ' ') + 1)._Ptr;
	}
	resource->SetName(
		std::format(L"StructuredBuffer-{}[{}, {}]", typeName, arraySize, numInstance).c_str()
	);
}

template<ConceptCPUBufferACE T>
inline void MdStructuredBuffer<T>::create_srv() {
	heapIndex = SRVDescriptorHeap::UseHeapIndex();
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = SRVDescriptorHeap::GetCPUHandle(heapIndex.value());
	gpuHandle = SRVDescriptorHeap::GetGPUHandle(heapIndex.value());

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = arraySize * numInstance;
	srvDesc.Buffer.StructureByteStride = sizeof(T);

	DxDevice::GetDevice()->CreateShaderResourceView(resource.Get(), &srvDesc, cpuHandle);
}

template<ConceptCPUBufferACE T>
inline void MdStructuredBuffer<T>::release_index() {
	if (heapIndex.has_value()) {
		SRVDescriptorHeap::ReleaseHeapIndex(heapIndex.value());
	}
}

template<ConceptCPUBufferACE T>
inline void MdStructuredBuffer<T>::map() {
	resource->Map(0, nullptr, reinterpret_cast<void**>(&data));
}

template<ConceptCPUBufferACE T>
inline void MdStructuredBuffer<T>::unmap() {
	if (data) {
		resource->Unmap(0, nullptr);
	}
	data = nullptr;
}
