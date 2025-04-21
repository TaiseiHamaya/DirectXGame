#pragma once

#include <format>
#include <typeinfo>

#include <Library/Utility/Tools/ConvertString.h>

#include "../ConceptCPUBuffer.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/DxResource.h"

template<ConceptCPUBufferACE T>
class ConstantBuffer : public DxResource {
public:
	ConstantBuffer() noexcept(false);
	ConstantBuffer(const T& data_) noexcept(false);

	virtual ~ConstantBuffer() noexcept;

	ConstantBuffer(ConstantBuffer&&) = default;
	ConstantBuffer& operator=(ConstantBuffer&&) = default;

public:
	const T* const get_data() const noexcept;
	T* const get_data() noexcept;
	void unmap();

protected:
	T* data;
	UINT memorySize;
};

template<ConceptCPUBufferACE T>
inline ConstantBuffer<T>::ConstantBuffer() noexcept(false) {
	memorySize = sizeof(T);
	resource = CreateBufferResource(memorySize);
	resource->Map(0, nullptr, reinterpret_cast<void**>(&data));
	*data = T{};
	std::wstring typeName = ConvertString(typeid(T).name());
	if constexpr (std::is_class_v<T> || std::is_enum_v<T>) {
		typeName = (std::find(typeName.begin(), typeName.end(), ' ') + 1)._Ptr;
	}
	resource->SetName(std::format(L"ConstantBuffer-{}", typeName).c_str());
}

template<ConceptCPUBufferACE T>
inline ConstantBuffer<T>::ConstantBuffer(const T& data_) noexcept(false) : ConstantBuffer<T>() {
	*data = data_;
}

template<ConceptCPUBufferACE T>
inline ConstantBuffer<T>::~ConstantBuffer() noexcept {
	unmap();
}

template<ConceptCPUBufferACE T>
inline const T* const ConstantBuffer<T>::get_data() const noexcept {
	return data;
}

template<ConceptCPUBufferACE T>
inline T* const ConstantBuffer<T>::get_data() noexcept {
	return data;
}

template<ConceptCPUBufferACE T>
inline void ConstantBuffer<T>::unmap() {
	if (data) {
		resource->Unmap(0, nullptr);
		data = nullptr;
	}
}
