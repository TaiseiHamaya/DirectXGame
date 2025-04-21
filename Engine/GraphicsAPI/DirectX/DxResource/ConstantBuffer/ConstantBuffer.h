#pragma once

#include <format>
#include <typeinfo>

#include <Library/Utility/Tools/ConvertString.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/DxResource.h"

template<typename T>
concept ConstantBufferConcept =
(std::is_arithmetic_v<T> || std::is_class_v<T> || std::is_enum_v<T>);

template<ConstantBufferConcept T>
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

template<ConstantBufferConcept T>
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

template<ConstantBufferConcept T>
inline ConstantBuffer<T>::ConstantBuffer(const T& data_) noexcept(false) : ConstantBuffer<T>() {
	*data = data_;
}

template<ConstantBufferConcept T>
inline ConstantBuffer<T>::~ConstantBuffer() noexcept {
	unmap();
}

template<ConstantBufferConcept T>
inline const T* const ConstantBuffer<T>::get_data() const noexcept {
	return data;
}

template<ConstantBufferConcept T>
inline T* const ConstantBuffer<T>::get_data() noexcept {
	return data;
}

template<ConstantBufferConcept T>
inline void ConstantBuffer<T>::unmap() {
	if (data) {
		resource->Unmap(0, nullptr);
		data = nullptr;
	}
}
