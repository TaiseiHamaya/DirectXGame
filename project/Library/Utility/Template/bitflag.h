#pragma once

#include <type_traits>

namespace eps {

template<typename T>
concept Enum = std::is_enum_v<T>;

template<Enum T>
class bitflag {
public: // using
	using value_type = T;
	using bitflag_type = bitflag<value_type>;
	using under_type = std::underlying_type<T>::type;

	using reference_bitflag = bitflag_type&;
	using const_reference_bitflag = const bitflag_type&;
	using right_reference_bifflag = bitflag_type&&;

public: // constructor/destructor
	constexpr bitflag();
	constexpr bitflag(const value_type& base);
	constexpr bitflag(const under_type& base);
	constexpr ~bitflag() = default;

	constexpr bitflag(const_reference_bitflag) = default;
	constexpr bitflag& operator=(const_reference_bitflag) = default;
	constexpr bitflag(right_reference_bifflag) = default;
	constexpr bitflag& operator=(right_reference_bifflag) = default;

public: // operator
	constexpr operator bool() const;

	constexpr bool operator==(const_reference_bitflag rhs) const;
	constexpr bool operator!=(const_reference_bitflag rhs) const;

	constexpr bitflag_type operator&=(const_reference_bitflag rhs);
	constexpr bitflag_type operator|=(const_reference_bitflag rhs);
	constexpr bitflag_type operator^=(const_reference_bitflag rhs);

	constexpr bitflag_type operator~() const;

private: // member function
	/// <summary>
	/// Enumを基底組み込み型に変換する
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	constexpr under_type to_under(const value_type& value);

private: // member value
	under_type value;
};

#define __USE_BITFLAG(EnumName) \
constexpr eps::bitflag<EnumName> operator|(const EnumName lhs, const EnumName rhs) {\
	using U = typename std::underlying_type<EnumName>::type;\
	return eps::bitflag<EnumName>(static_cast<U>(lhs) | static_cast<U>(rhs));\
}\
\
constexpr eps::bitflag<EnumName> operator&(const EnumName lhs, const EnumName rhs) {\
	using U = typename std::underlying_type<EnumName>::type;\
	return eps::bitflag<EnumName>(static_cast<U>(lhs) & static_cast<U>(rhs));\
}\
\
constexpr eps::bitflag<EnumName> operator^(const EnumName lhs, const EnumName rhs) {\
	using U = typename std::underlying_type<EnumName>::type;\
	return eps::bitflag<EnumName>(static_cast<U>(lhs) ^ static_cast<U>(rhs));\
}

// -------------------------------------
// ---------- public function ----------
// -------------------------------------

/// <summary>
/// ビットフラグ型に変換する
/// </summary>
/// <typeparam name="T">Enum</typeparam>
/// <param name="value"></param>
/// <returns></returns>
template<Enum T>
constexpr bitflag<T> to_bitflag(const T& value) {
	return bitflag<T>(value);
}

// ---------------------------------
// ---------- constructor ----------
// ---------------------------------

template<Enum T>
inline constexpr bitflag<T>::bitflag() : value(0) {
}

template<Enum T>
inline constexpr bitflag<T>::bitflag(const value_type& base) : value(to_under(base)) {
}

template<Enum T>
inline constexpr bitflag<T>::bitflag(const under_type& base) : value(base) {
}

// -------------------------------------
// ---------- public operator ----------
// -------------------------------------

template<Enum T>
inline constexpr bitflag<T>::operator bool() const {
	return value;
}

template<Enum T>
inline constexpr bool bitflag<T>::operator==(const_reference_bitflag rhs) const {
	return value == rhs.value;
}

template<Enum T>
inline constexpr bool bitflag<T>::operator!=(const_reference_bitflag rhs) const {
	return !(*this == rhs);
}

template<Enum T>
inline constexpr bitflag<T> bitflag<T>::operator&=(const_reference_bitflag rhs) {
	value &= rhs.value;
	return *this;
}

template<Enum T>
inline constexpr bitflag<T> bitflag<T>::operator|=(const_reference_bitflag rhs) {
	value |= rhs.value;
	return *this;
}

template<Enum T>
inline constexpr bitflag<T> bitflag<T>::operator^=(const_reference_bitflag rhs) {
	value ^= rhs.value;
	return *this;
}

template<Enum T>
inline constexpr bitflag<T> bitflag<T>::operator~() const {
	return ~value;
}

// --------------------------------------
// ---------- private function ----------
// --------------------------------------

template<Enum T>
inline constexpr bitflag<T>::under_type bitflag<T>::to_under(const value_type& value) {
	return static_cast<bitflag<T>::under_type>(value);
}

// -------------------------------------
// ---------- gloval operator ----------
// -------------------------------------

template<Enum T>
constexpr bitflag<T> operator&(const bitflag<T>& lhs, const bitflag<T>& rhs) {
	return bitflag<T>(lhs) &= rhs;
}

template<Enum T>
constexpr bitflag<T> operator&(const bitflag<T>& lhs, const T& rhs) {
	return lhs & to_bitflag<T>(rhs);
}

template<Enum T>
constexpr bitflag<T> operator&(const T& lhs, const bitflag<T>& rhs) {
	return rhs & lhs;
}

template<Enum T>
constexpr bitflag<T> operator|(const bitflag<T>& lhs, const bitflag<T>& rhs) {
	return bitflag<T>(lhs) |= rhs;
}

template<Enum T>
constexpr bitflag<T> operator|(const bitflag<T>& lhs, const T& rhs) {
	return lhs | to_bitflag(rhs);
}

template<Enum T>
constexpr bitflag<T> operator|(const T& lhs, const bitflag<T>& rhs) {
	return rhs | lhs;
}

template<Enum T>
constexpr bitflag<T> operator^(const bitflag<T>& lhs, const bitflag<T>& rhs) {
	return bitflag<T>(lhs) ^= rhs;
}

template<Enum T>
constexpr bitflag<T> operator^(const bitflag<T>& lhs, const T& rhs) {
	return lhs ^ to_bitflag(rhs);
}

template<Enum T>
constexpr bitflag<T> operator^(const T& lhs, const bitflag<T>& rhs) {
	return rhs ^ lhs;
}

} // namespace eps
