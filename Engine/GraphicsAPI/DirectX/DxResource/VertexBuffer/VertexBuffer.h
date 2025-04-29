#pragma once

#include "Engine/GraphicsAPI/DirectX/DxResource/DxResource.h"

#include <concepts>
#include <type_traits>

#include "../ConceptCPUBuffer.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h"

template<class Array, typename T>
concept WriteableVertexBuffer =
// 連続メモリ配置またはイニシャライザーリスト
(std::contiguous_iterator<typename Array::iterator> || std::same_as<Array, std::initializer_list<T>>) &&
std::same_as<typename Array::value_type, T>&& // Arrayのtemplate型がTと等しい
	requires(Array array) {
		{ array.size() } -> std::convertible_to<size_t>; // size()が利用可能で、size_tに変換可能
		{ std::to_address(array.begin()) } -> std::convertible_to<const typename Array::value_type*>; // 先頭アドレス取得関数が存在し、それがconst T::value_type*に変換可能
};

template<ConceptCPUBufferAC T>
class VertexBuffer final : public DxResource {
public:
	VertexBuffer() noexcept = default;
	template<WriteableVertexBuffer<T> Array>
	VertexBuffer(const Array& vertices_array) noexcept(false);
	VertexBuffer(u32 size_) noexcept(false);
	~VertexBuffer() noexcept = default;

	VertexBuffer(VertexBuffer&&) = default;
	VertexBuffer& operator=(VertexBuffer&&) = default;

public:
	const D3D12_VERTEX_BUFFER_VIEW& get_vbv() const noexcept { return vertexBufferView; };
	const u32 vertex_size() const noexcept { return size; };
	template<WriteableVertexBuffer<T> Array>
	void write(const Array& vertices_array);

private:
	void set_size(u32 size_);
	void create_resource();
	void unmap();

private:
	T* data;
	u32 size;
	UINT memorySize;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	static constexpr UINT VERTEX_DATA_SIZE = sizeof(T);
};

template<ConceptCPUBufferAC T>
template<WriteableVertexBuffer<T> Array>
inline VertexBuffer<T>::VertexBuffer(const Array& vertices_array) noexcept(false) {
	write(vertices_array);
}

template<ConceptCPUBufferAC T>
template<WriteableVertexBuffer<T> Array>
inline void VertexBuffer<T>::write(const Array& vertices_array) {
	u32 arraySize = static_cast<u32>(vertices_array.size());
	// リソースがnullptrなら作成
	// すでに確保された容量と違う場合、再生成
	if (!resource || size != arraySize) {
		set_size(arraySize);
		create_resource();
	}
	// Map
	resource->Map(0, nullptr, reinterpret_cast<void**>(&data));
	// コピー
	std::memcpy(data, std::to_address(vertices_array.begin()), memorySize);
	unmap();
}

template<ConceptCPUBufferAC T>
inline VertexBuffer<T>::VertexBuffer(u32 size_) noexcept(false) {
	set_size(size_);
	create_resource();
}

template<ConceptCPUBufferAC T>
inline void VertexBuffer<T>::set_size(u32 size_) {
	size = size_;
	memorySize = size * VERTEX_DATA_SIZE;
}

template<ConceptCPUBufferAC T>
inline void VertexBuffer<T>::create_resource() {
	resource = CreateBufferResource(memorySize);
	vertexBufferView.BufferLocation = resource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = memorySize;
	vertexBufferView.StrideInBytes = VERTEX_DATA_SIZE;
}

template<ConceptCPUBufferAC T>
inline void VertexBuffer<T>::unmap() {
	if (data) {
		resource->Unmap(0, nullptr);
		data = nullptr;
	}
}

using Object3DVertexBuffer = VertexBuffer<VertexDataBuffer>;
