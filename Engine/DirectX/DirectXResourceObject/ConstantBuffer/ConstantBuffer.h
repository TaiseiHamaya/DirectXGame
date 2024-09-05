#pragma once

#include "Engine/DirectX/DirectXResourceObject/DirectXResourceObject.h"

template<typename T>
class ConstantBuffer : public DirectXResourceObject {
public:
	ConstantBuffer() noexcept(false);
	ConstantBuffer(const T& data_) noexcept(false);

	virtual ~ConstantBuffer() noexcept;

	ConstantBuffer(ConstantBuffer&&) = default;
	ConstantBuffer& operator=(ConstantBuffer&&) = default;

public:
	const T* const get_data() const noexcept;
	T* const get_data() noexcept;

protected:
	T* data;
	UINT memorySize;
};

template<typename T>
inline ConstantBuffer<T>::ConstantBuffer() noexcept(false) {
	memorySize = sizeof(T);
	resource = CreateBufferResource(memorySize);
	resource->Map(0, nullptr, reinterpret_cast<void**>(&data));
}

template<typename T>
inline ConstantBuffer<T>::ConstantBuffer(const T& data_) noexcept(false) : ConstantBuffer<T>() {
	*data = data_;
}

template<typename T>
inline ConstantBuffer<T>::~ConstantBuffer() noexcept {
	if (resource) {
		resource->Unmap(0, nullptr);
	}
}

template<typename T>
inline const T* const ConstantBuffer<T>::get_data() const noexcept {
	return data;
}

template<typename T>
inline T* const ConstantBuffer<T>::get_data() noexcept {
	return data;
}
