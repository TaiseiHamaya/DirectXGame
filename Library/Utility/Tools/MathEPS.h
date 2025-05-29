#pragma once

#include <bit>
#include <type_traits>

namespace eps {

/// <summary>
/// 線形補間
/// </summary>
/// <returns></returns>
template<typename T, T one = 1.0f>
constexpr T lerp(const T& from, const T& to, r32 t) noexcept;

/// <summary>
/// 逆線形補間
/// </summary>
template<typename T>
constexpr r32 lerp_inv(const T& from, const T& to, const T& now) noexcept;

/// <summary>
/// 整数乗関数(繰り返し二乗法)
/// </summary>
template<typename T>
	requires std::is_arithmetic_v<T>
constexpr T pow(T base, u64 exponent);

/// <summary>
/// [-1-1]で小数部の取得
/// </summary>
constexpr r32 fractional(r32 value) noexcept;

/// <summary>
/// 入力を[0-1]にClampする
/// </summary>
/// <param name="value"></param>
/// <returns></returns>
constexpr r32 saturate(r32 value) noexcept;

}

template<typename T, T one>
constexpr T eps::lerp(const T& begin, const T& end, r32 t) noexcept {
	return T(begin * (one - t)) + T(end * t);
}

template<typename T>
constexpr r32 eps::lerp_inv(const T& begin, const T& end, const T& now) noexcept {
	if (begin == end) {
		return 0.0f;
	}
	return (now - begin) / (end - begin);
}

template<typename T>
	requires std::is_arithmetic_v<T>
constexpr T eps::pow(T base, u64 exponent) {
	T result{ 1 };
	while (exponent) {
		if (exponent & 0b1) {
#ifdef DEBUG_FEATURES_ENABLE
			// resultオーバーフローチェック
#endif // DEBUG_FEATURES_ENABLE

			result *= base;
		}
#ifdef DEBUG_FEATURES_ENABLE
		// baseオーバーフローチェック
#endif // DEBUG_FEATURES_ENABLE
		base *= base;
		exponent >>= 1;
	}
	return result;
}

constexpr r32 eps::fractional(r32 value) noexcept {
	return value - i32(value);
}

constexpr r32 eps::saturate(r32 value) noexcept {
	// 詳細 : http://cppdiary.blog76.fc2.com/blog-entry-31.html

	i32 calc = std::bit_cast<i32, r32>(value);
	calc &= static_cast<i32>(calc ^ 0x80000000) >> 31;
	calc -= 0x3F800000;
	calc &= calc >> 31;
	calc += 0x3F800000;
	return std::bit_cast<r32, i32>(calc);
}
