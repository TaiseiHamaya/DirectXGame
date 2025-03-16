#pragma once

#include <concepts>
#include <cstdint>
#include <iterator>
#include <type_traits>

#include "Engine/GraphicsAPI/DirectX/DxResource/DxResource.h"

template <class T>
concept WriteableIndexBuffer =
// 連続メモリ配置またはイニシャライザーリスト
(std::contiguous_iterator<typename T::iterator> || std::same_as<T, std::initializer_list<typename T::value_type>>) &&
std::same_as<typename T::value_type, std::uint32_t>&& // template型がuint32_t
	requires(T v) {
		{ v.size() } -> std::convertible_to<std::size_t>; // size()が利用可能で、size_tに変換可能
		{ std::to_address(v.begin()) } -> std::convertible_to<const typename T::value_type*>; // 先頭アドレス取得関数が存在し、それがconst T::value_type*に変換可能
};

class IndexBuffer final : public DxResource {
public:
	IndexBuffer() noexcept = default;
	template<typename T>
	IndexBuffer(const T& indexes_) noexcept(false);
	//IndexBuffer(const std::vector<uint32_t>& indexes_) noexcept(false);
	IndexBuffer(std::uint32_t size) noexcept(false);
	~IndexBuffer() noexcept = default;

	IndexBuffer(IndexBuffer&&) = default;
	IndexBuffer& operator=(IndexBuffer&&) = default;

public:
	const D3D12_INDEX_BUFFER_VIEW* const get_p_ibv() const noexcept;
	const std::uint32_t index_size() const noexcept;
	template<WriteableIndexBuffer T>
	void write(const T& indexes_array);

private:
	void set_size(std::uint32_t size_);
	void create_resource();
	void unmap();

private:
	std::uint32_t* data;
	std::uint32_t size{};
	UINT memorySize;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
};

template<typename T>
inline IndexBuffer::IndexBuffer(const T& indexes_) noexcept(false) {
	write(indexes_);
}

template<WriteableIndexBuffer T>
inline void IndexBuffer::write(const T& indexes_array) {
	std::uint32_t arraySize = static_cast<std::uint32_t>(indexes_array.size());
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
