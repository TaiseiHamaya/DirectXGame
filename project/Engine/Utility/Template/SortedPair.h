#pragma once

#include <concepts>
#include <utility>

#include "Engine/Utility/Tools/Hash.h"

#undef small
#undef big

/// <summary>
/// ソート済みペア
/// テンプレート引数制約 : 比較演算可能である
/// </summary>
/// <typeparam name="T"></typeparam>
template<std::totally_ordered T>
class SortedPair {
public:
	SortedPair() = default;
	~SortedPair() = default;

	SortedPair(const SortedPair&) = default;
	SortedPair& operator=(const SortedPair&) = default;
	SortedPair(SortedPair&&) = default;
	SortedPair& operator=(SortedPair&&) = default;

public:
	bool operator==(const SortedPair& rhs) const {
		return values.first == rhs.values.first && values.second == rhs.values.second;
	}
	bool operator!=(const SortedPair& rhs) const {
		return !(*this == rhs);
	}

public:
	SortedPair(const T& val1, const T& val2);

public:
	const T& small() const { return values.first; };
	const T& big() const { return values.second; };

private:
	std::pair<T, T> values;
};

template<std::totally_ordered T>
inline SortedPair<T>::SortedPair(const T& val1, const T& val2) :
	values(
		val1 <= val2 ?
		std::make_pair(val1, val2) :
		std::make_pair(val2, val1)
	) {
}

namespace std {

template<typename T>
struct hash<SortedPair<T>> {
public:
	size_t operator()(const SortedPair<T>& val) const {
		return eps::hash_vector<T>({ val.big(), val.small() });
	}
};

}
