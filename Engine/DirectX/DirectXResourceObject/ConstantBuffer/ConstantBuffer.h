#pragma once

#include "Engine/DirectX/DirectXResourceObject/DirectXResourceObject.h"

template<typename T>
class ConstantBuffer : public DirectXResourceObject {
public:
	ConstantBuffer();
	ConstantBuffer(const T& data_);

	virtual ~ConstantBuffer();

protected:
	T* data;
	UINT memorySize;
};

template<typename T>
inline ConstantBuffer<T>::ConstantBuffer() {
	memorySize = sizeof(T);
	resource = CreateBufferResource(memorySize);
	resource->Map(0, nullptr, reinterpret_cast<void**>(&data));
}

template<typename T>
inline ConstantBuffer<T>::ConstantBuffer(const T& data_) : ConstantBuffer<T>() {
	*data = data_;
}

template<typename T>
inline ConstantBuffer<T>::~ConstantBuffer() {
	resource->Unmap(0, nullptr);
}
