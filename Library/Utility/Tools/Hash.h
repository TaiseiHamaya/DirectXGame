#pragma once

#include <bit>
#include <initializer_list>
#include <type_traits>

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
inline u64 hash(u64 seed, u64 value) {
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
u64 hash_vector(const Array& array) {
	u64 result = array.size();
	u64 value;
	for (auto itr = std::begin(array); itr != std::end(array); ++itr) {
		value = _compression_64bit<typename Array::value_type>(*itr);
		result = hash(result, value);
	}
	return result;
}

template<typename T>
u64 hash_vector(std::initializer_list<T>&& initializerList) {
	u64 result = initializerList.size();
	u64 value;
	for (auto itr : initializerList) {
		value = _compression_64bit<T>(itr);
		result = hash(result, value);
	}
	return result;
}

}