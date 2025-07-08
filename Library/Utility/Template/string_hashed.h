#pragma once

#include <compare>
#include <string_view>

#include "Library/Utility/Tools/ConstructorMacro.h"
#include "Library/Utility/Tools/Hash.h"

#ifdef DEBUG_FEATURES_ENABLE
#include <string>
#endif

namespace eps {

#ifdef DEBUG_FEATURES_ENABLE

class string_hashed {
public:
	string_hashed() = default;
	~string_hashed() = default;

	__CLASS_DEFAULT_ALL(string_hashed)

public:
	string_hashed(std::string_view sv);
	string_hashed& operator=(std::string_view sv);
	string_hashed(string_literal sl);
	string_hashed& operator=(string_literal sl);

	bool operator==(const string_hashed& rhs) const;
	std::strong_ordering operator<=>(const string_hashed& rhs) const;

public:
	u64 value() const;

	// デバッグ時のみ使用可能
	std::string_view view_debug() const;

private:
	u64 val{};
	std::string str;
};

#else

class string_hashed {
public:
	constexpr string_hashed() = default;
	constexpr ~string_hashed() = default;

	__CLASS_DEFAULT_ALL(string_hashed)

public:
	constexpr string_hashed(std::string_view sv);
	constexpr string_hashed& operator=(std::string_view sv);
	constexpr string_hashed(string_literal sl);
	constexpr string_hashed& operator=(string_literal sl);

	constexpr bool operator==(const string_hashed& rhs) const;
	constexpr std::strong_ordering operator<=>(const string_hashed& rhs) const;

public:
	constexpr u64 value() const;

private:
	u64 val{};
};

#endif //DEBUG_FEATURES_ENABLE

};

#ifdef DEBUG_FEATURES_ENABLE

namespace std {
// C++ハッシュ関数の特殊化
template<>
struct hash<eps::string_hashed> {
	u64 operator()(const eps::string_hashed& a) const {
		return a.value();
	}
};

};

// string_hashedの文字列リテラル
inline eps::string_hashed operator"" _sh(string_literal str, std::size_t len) {
	return str;
}

#else

constexpr eps::string_hashed::string_hashed(std::string_view sv) {
	val = eps::string_hash(sv);
}

constexpr eps::string_hashed& eps::string_hashed::operator=(std::string_view sv) {
	val = eps::string_hash(sv);
	return *this;
}

constexpr eps::string_hashed::string_hashed(string_literal sv) {
	val = eps::string_hash(sv);
}

constexpr eps::string_hashed& eps::string_hashed::operator=(string_literal sv) {
	val = eps::string_hash(sv);
	return *this;
}

constexpr u64 eps::string_hashed::value() const {
	return val;
}

constexpr bool eps::string_hashed::operator==(const string_hashed& rhs) const {
	return val == rhs.val;
}

constexpr std::strong_ordering eps::string_hashed::operator<=>(const string_hashed& rhs) const {
	return val <=> rhs.val;
}

namespace std {

// C++ハッシュ関数の特殊化
template<>
struct hash<eps::string_hashed> {
	constexpr u64 operator()(const eps::string_hashed& a) const {
		return a.value();
	}
};

};

constexpr eps::string_hashed operator""_sh(string_literal str, std::size_t len) {
	return str;
}

#endif
