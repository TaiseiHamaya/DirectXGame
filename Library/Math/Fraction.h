#pragma once

#include <concepts>
#include <string>

namespace __internal {

template<std::signed_integral T>
constexpr T __gcd(T a, T b) {
	if (a % b == 0)
		return b;
	return __gcd<T>(b, a % b);
}

template<std::signed_integral T>
constexpr T gcd(T a, T b) {
	if (b == 0)
		return (a >= 0 ? a : -a);
	return __gcd<T>((a >= 0 ? a : -a), (b >= 0 ? b : -b));
}

template<std::signed_integral T>
constexpr T lcm(T a, T b) {
	a = std::abs(a);
	b = std::abs(b);
	if (a > b) std::swap(a, b);

	if (a == 1) return b;

	T result = a;
	while (result % b != 0) {
		result += a;
	}
	return result;
}

template<std::signed_integral T>
constexpr void simplify(T& num, T& den) {
	T d = gcd<T>(num, den);
	num /= (den >= 0 ? d : -d);
	den /= (den >= 0 ? d : -d);
}

};

/// <summary>
/// 分数ライブラリ
/// 
/// 参照リンク
/// https://qiita.com/gandalfr-KY/items/b0eed6727e873a313455
/// </summary>
/// <typeparam name="T></typeparam>
template<std::signed_integral T = i32>
class Fraction {
public: // using
	using value_type = T;
	using fraction_type = Fraction<value_type>;

	using reference_fraction = fraction_type&;
	using const_reference_fraction = const fraction_type&;
	using right_reference_fraction = fraction_type&&;

public:
	constexpr Fraction() noexcept = default;
	~Fraction() = default;

	constexpr Fraction(value_type numer_, value_type denom_) noexcept;
	constexpr Fraction(value_type value) noexcept;

public:
	constexpr Fraction(const Fraction&) noexcept = default;
	constexpr Fraction& operator=(const Fraction&) noexcept = default;
	constexpr Fraction(Fraction&&) noexcept = default;
	constexpr Fraction& operator=(Fraction&&) noexcept = default;

public:
	constexpr operator bool() const;

	constexpr fraction_type& operator+();
	constexpr fraction_type operator-() const;

	constexpr reference_fraction operator+=(const_reference_fraction rhs);
	constexpr reference_fraction operator-=(const_reference_fraction rhs);
	constexpr reference_fraction operator*=(const_reference_fraction rhs);
	constexpr reference_fraction operator/=(const_reference_fraction rhs);

public:
	constexpr fraction_type inverse() const;
	template<std::floating_point real = r32>
	constexpr real to_real() const;
	constexpr fraction_type abs() const;
	constexpr value_type lcm() const;
	std::string to_string() const;

public:
	value_type numer{ 0 };
	value_type denom{ 1 };
};

template<std::signed_integral T>
inline constexpr Fraction<T>::Fraction(value_type numer_, value_type denom_) noexcept :
	numer(numer_),
	denom(denom_) {
	__internal::simplify(numer, denom);
}

template<std::signed_integral T>
inline constexpr Fraction<T>::Fraction(value_type value) noexcept : Fraction(value, 1) {
}

template<std::signed_integral T>
inline constexpr Fraction<T>::operator bool() const {
	return numer;
}

template<std::signed_integral T>
inline constexpr Fraction<T>::fraction_type& Fraction<T>::operator+() {
	return *this;
}

template<std::signed_integral T>
inline constexpr Fraction<T>::fraction_type Fraction<T>::operator-() const {
	return fraction_type(-numer, denom);
}

template<std::signed_integral T>
inline constexpr Fraction<T> operator+(const Fraction<T>& lhs, const Fraction<T>& rhs) {
	Fraction<T>(lhs.numer * rhs.denom + lhs.denom * rhs.numer, lhs.denom * rhs.denom);
}

template<std::signed_integral T>
inline constexpr Fraction<T> operator-(const Fraction<T>& lhs, const Fraction<T>& rhs) {
	return lhs + (-rhs);
}

template<std::signed_integral T>
inline constexpr Fraction<T> operator*(const Fraction<T>& lhs, const Fraction<T>& rhs) {
	T gcdL = __internal::gcd(lhs.numer, rhs.denom);
	T gcdR = __internal::gcd(rhs.numer, lhs.denom);
	return Fraction<T>((lhs.numer / gcdL) * (rhs.numer / gcdR), (lhs.denom / gcdR) * (rhs.denom / gcdL));
}

template<std::signed_integral T>
inline constexpr Fraction<T> operator/(const Fraction<T>& lhs, const Fraction<T>& rhs) {
	return lhs * rhs.inverse();
}

template<std::signed_integral T>
inline constexpr Fraction<T>::reference_fraction Fraction<T>::operator+=(const_reference_fraction rhs) {
	*this = *this + rhs;
	return *this;
}

template<std::signed_integral T>
inline constexpr Fraction<T>::reference_fraction Fraction<T>::operator-=(const_reference_fraction rhs) {
	*this = *this - rhs;
	return *this;
}

template<std::signed_integral T>
inline constexpr Fraction<T>::reference_fraction Fraction<T>::operator*=(const_reference_fraction rhs) {
	*this = *this * rhs;
	return *this;
}

template<std::signed_integral T>
inline constexpr Fraction<T>::reference_fraction Fraction<T>::operator/=(const_reference_fraction rhs) {
	*this = *this / rhs;
	return *this;
}

template<std::signed_integral T>
inline constexpr Fraction<T>::fraction_type Fraction<T>::inverse() const {
	return fraction_type{ denom, numer };
}

template<std::signed_integral T>
inline constexpr Fraction<T>::fraction_type Fraction<T>::abs() const {
	return fraction_type(std::abs(numer), denom);
}

template<std::signed_integral T>
inline constexpr Fraction<T>::value_type Fraction<T>::lcm() const {
	return numer * denom;
}

template<std::signed_integral T>
inline std::string Fraction<T>::to_string() const {
	if (denom == 1) {
		return std::to_string(numer);
	}
	else {
		return std::to_string(numer) + '/' + std::to_string(denom);
	}
}

template<std::signed_integral T>
template<std::floating_point real>
inline constexpr real Fraction<T>::to_real() const {
	return static_cast<real>(numer) / denom;
}

template<std::signed_integral T>
inline constexpr bool operator==(const Fraction<T>& lhs, const Fraction<T>& rhs) {
	return lhs.numer == rhs.numer && lhs.denom == rhs.denom;
}

template<std::signed_integral T>
inline constexpr bool operator!=(const Fraction<T>& lhs, const Fraction<T>& rhs) {
	return !(*this == rhs);
}

template<std::signed_integral T>
inline constexpr bool operator<(const Fraction<T>& lhs, const Fraction<T>& rhs) {
	T vlcm = __internal::lcm<T>(lhs.denom, rhs.denom);
	return vlcm * lhs.numer < vlcm * rhs.numer;
}

template<std::signed_integral T>
inline constexpr bool operator>(const Fraction<T>& lhs, const Fraction<T>& rhs) {
	return !(lhs <= rhs);
}

template<std::signed_integral T>
inline constexpr bool operator<=(const Fraction<T>& lhs, const Fraction<T>& rhs) {
	return lhs < rhs || lhs == rhs;
}

template<std::signed_integral T>
inline constexpr bool operator>=(const Fraction<T>& lhs, const Fraction<T>& rhs) {
	return !(lhs < rhs);
}

template<std::signed_integral T>
std::istream& operator>>(std::istream& is, Fraction<T>& value) {
	std::string buf;
	is >> buf;
	T num_tmp = 0, den_tmp = 0;
	size_t i = (buf[0] == '-'), sz = buf.size();
	for (; i < sz && buf[i] != '/'; i++)
		num_tmp = num_tmp * 10 + buf[i] - '0';
	if (i == sz)
		den_tmp = 1;
	else
		for (i = i + 1; i < sz; i++)
			den_tmp = den_tmp * 10 + buf[i] - '0';
	if (buf[0] == '-')
		num_tmp *= -1;
	__internal::simplify(num_tmp, den_tmp);
	value.numer = num_tmp, value.denom = den_tmp;
	return is;
}

template<std::signed_integral T>
std::ostream& operator<<(std::ostream& os, Fraction<T>& value) {
	os << value.to_string();
	return os;
}