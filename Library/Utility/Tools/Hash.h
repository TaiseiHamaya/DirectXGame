#pragma once

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
inline size_t hash(size_t seed, size_t value) {
	value = ((value >> 16) ^ value) * 0x45d9f3b;
	value = ((value >> 16) ^ value) * 0x45d9f3b;
	value = (value >> 16) ^ value;
	seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	return seed;
}

template<typename T>
constexpr size_t _compression_64bit(const T& value) {
	size_t result;
	// sizeof(size_t)以下
	if constexpr ((sizeof(T) <= sizeof(size_t))) {
		// バイナリデータを強制的にsize_tに変換
		result = *reinterpret_cast<const size_t*>(&value);
	}
	else {
		// 1度ハッシュ化して64bitに圧縮
		// TがCV修飾の場合、定義なしになるのでCV修飾を外す
		result = std::hash<std::remove_cv_t<T>>()(value);
	}
	return result;
}

template<typename Array>
size_t hash_vector(const Array& array) {
	size_t result = array.size();
	size_t value;
	for (auto itr = std::begin(array); itr != std::end(array); ++itr) {
		value = _compression_64bit<typename Array::value_type>(*itr);
		result = hash(result, value);
	}
	return result;
}

template<typename T>
size_t hash_vector(std::initializer_list<T>&& initializerList) {
	size_t result = initializerList.size();
	size_t value;
	for (auto itr : initializerList) {
		value = _compression_64bit<T>(itr);
		result = hash(result, value);
	}
	return result;
}

}