#pragma once

#include <bit>
#include <initializer_list>
#include <type_traits>
#include <vector>

namespace eps {

/// <summary>
/// <para>
/// 複数パラメータをもつハッシュ値生成
/// </para>
/// <para>
/// <see href="https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector/72073933#72073933">詳細</see>
/// </para>
/// </summary>
/// <param name="seed"></param>
/// <param name="value"></param>
constexpr u64 hash(u64 seed, u64 value) noexcept {
	value = ((value >> 16) ^ value) * 0x45d9f3b;
	value = ((value >> 16) ^ value) * 0x45d9f3b;
	value = (value >> 16) ^ value;
	seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	return seed;
}

template<typename T>
constexpr u64 _compression_64bit(const T& value) {
	u64 result;
	if constexpr (sizeof(T) == sizeof(u64)) {
		// バイナリデータを強制的に64bitに変換
		result = std::bit_cast<u64, T>(value);
	}
	else if constexpr (sizeof(T) == sizeof(u32)) {
		result = std::bit_cast<u32, T>(value);
	}
	else if constexpr (sizeof(T) == sizeof(u16)) {
		result = std::bit_cast<u16, T>(value);
	}
	else if constexpr (sizeof(T) == sizeof(u8)) {
		result = std::bit_cast<u8, T>(value);
	}
	else {
		// 1度ハッシュ化して64bitに圧縮
		// TがCV修飾の場合、定義なしになるのでCV修飾を外す
		result = std::hash<std::remove_cv_t<T>>()(value);
	}
	return result;
}

template<typename Array>
constexpr u64 hash_vector(const Array& array) {
	u64 result = array.size();
	for (auto itr = std::begin(array); itr != std::end(array); ++itr) {
		u64 value = _compression_64bit<typename Array::value_type>(*itr);
		result = hash(result, value);
	}
	return result;
}

template<typename T>
constexpr u64 hash_vector(std::initializer_list<T>&& initializerList) {
	u64 result = initializerList.size();
	for (auto& itr : initializerList) {
		u64 value = _compression_64bit<T>(itr);
		result = hash(result, value);
	}
	return result;
}

constexpr u64 string_hash(std::string_view str) noexcept {
	constexpr u64 u8BitSize = sizeof(u8) * 8;

	std::vector<u64> values;
	u64 value{ 0 };
	for (i32 i = 0; auto& c : str) {
		value |= static_cast<u64>(c) << (i * u8BitSize);
		++i;
		if (sizeof(u64) * 8 / u8BitSize == i) {
			values.emplace_back(value);
			value = 0;
		}
	}
	if (value != 0) {
		values.emplace_back(value);
	}

	u64 result = 0;
	for (auto& elem : values) {
		result = hash(result, elem);
	}

	return result;
}

}
