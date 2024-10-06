#pragma once

#include <typeinfo>
#include <format>

#include "Engine/DirectX/DirectXResourceObject/DirectXResourceObject.h"
#include "Engine/Utility/Utility.h"

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
	void unmap();

protected:
	T* data;
	UINT memorySize;
};

template<typename T>
inline ConstantBuffer<T>::ConstantBuffer() noexcept(false) {
	memorySize = sizeof(T);
	resource = CreateBufferResource(memorySize);
	resource->Map(0, nullptr, reinterpret_cast<void**>(&data));
	*data = T{};
	std::wstring typeName = ConvertString(typeid(T).name());
	auto begin = std::find(typeName.begin(), typeName.end(), ' ') + 1;
	resource->SetName(std::format(L"ConstantBuffer-{}", begin._Ptr).c_str());
}

template<typename T>
inline ConstantBuffer<T>::ConstantBuffer(const T& data_) noexcept(false) : ConstantBuffer<T>() {
	*data = data_;
}

template<typename T>
inline ConstantBuffer<T>::~ConstantBuffer() noexcept {
	unmap();
}

template<typename T>
inline const T* const ConstantBuffer<T>::get_data() const noexcept {
	return data;
}

template<typename T>
inline T* const ConstantBuffer<T>::get_data() noexcept {
	return data;
}

template<typename T>
inline void ConstantBuffer<T>::unmap() {
	if (data) {
		resource->Unmap(0, nullptr);
		data = nullptr;
	}
}
