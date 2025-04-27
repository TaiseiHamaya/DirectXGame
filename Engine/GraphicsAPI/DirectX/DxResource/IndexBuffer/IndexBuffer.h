#pragma once

#include <concepts>
#include <memory>
#include <type_traits>

#include "Engine/GraphicsAPI/DirectX/DxResource/DxResource.h"

template <class T>
concept WriteableIndexBuffer =
// 連続メモリ配置またはイニシャライザーリスト
(std::contiguous_iterator<typename T::iterator> || std::same_as<T, std::initializer_list<typename T::value_type>>) &&
std::same_as<typename T::value_type, u32>&& // template型がu32
	requires(T v) {
		{ v.size() } -> std::convertible_to<size_t>; // size()が利用可能で、size_tに変換可能
		{ std::to_address(v.begin()) } -> std::convertible_to<const typename T::value_type*>; // 先頭アドレス取得関数が存在し、それがconst T::value_type*に変換可能
};

class IndexBuffer final : public DxResource {
public:
	IndexBuffer() noexcept = default;
	template<typename T>
	IndexBuffer(const T& indexes_) noexcept(false);
	//IndexBuffer(const std::vector<u32>& indexes_) noexcept(false);
	IndexBuffer(u32 size) noexcept(false);
	~IndexBuffer() noexcept = default;

	IndexBuffer(IndexBuffer&&) = default;
	IndexBuffer& operator=(IndexBuffer&&) = default;

public:
	const D3D12_INDEX_BUFFER_VIEW* const get_p_ibv() const noexcept;
	const u32 index_size() const noexcept;
	template<WriteableIndexBuffer T>
	void write(const T& indexes_array);

private:
	void set_size(u32 size_);
	void create_resource();
	void unmap();

private:
	u32* data;
	u32 size{};
	UINT memorySize;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
};

template<typename T>
inline IndexBuffer::IndexBuffer(const T& indexes_) noexcept(false) {
	write(indexes_);
}

template<WriteableIndexBuffer T>
inline void IndexBuffer::write(const T& indexes_array) {
	u32 arraySize = static_cast<u32>(indexes_array.size());
	// リソースがnullptrなら作成
	// すでに確保された容量と違う場合、再生成
	if (!resource || size != arraySize) {
		set_size(arraySize);
		create_resource();
	}
	// Map
	resource->Map(0, nullptr, reinterpret_cast<void**>(&data));
	// コピー
	std::memcpy(data, std::to_address(indexes_array.begin()), memorySize);
	unmap();
}
