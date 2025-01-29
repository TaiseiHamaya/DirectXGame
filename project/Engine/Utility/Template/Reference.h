#pragma once

#include <memory>
#include <type_traits>

template<typename T>
class Reference {
public:
	Reference() noexcept = default;
	~Reference() noexcept = default;

	Reference(const Reference<T>&) noexcept = default;
	Reference& operator=(const Reference<T>&) noexcept = default;
	Reference(Reference<T>&&) noexcept = default;
	Reference& operator=(Reference<T>&&) noexcept = default;

public: // -------------------- T, T*, unique<T>からの暗黙代入 --------------------
	Reference(T& rhs) noexcept : reference(&rhs) {};
	Reference(T* const rhs) noexcept : reference(rhs) {};
	Reference(const std::unique_ptr<T>& rhs) noexcept : reference(rhs.get()) {};
	Reference& operator=(T& rhs) noexcept {
		reference = &rhs;
		return *this;
	};
	Reference& operator=(T* const rhs) noexcept {
		reference = rhs;
		return *this;
	};
	Reference& operator=(const std::unique_ptr<T>& rhs) noexcept {
		reference = rhs.get();
		return *this;
	};

public: // -------------------- 型変換付きキャスト --------------------
	template<std::convertible_to<T> U>
	Reference(Reference<U> rhs) noexcept { reference = rhs.ptr(); };
	template<std::convertible_to<T> U>
	Reference(U* const rhs) noexcept { reference = rhs; };
	template<std::convertible_to<T> U>
	Reference(const std::unique_ptr<U>& rhs) noexcept { reference = rhs.get(); };
	template<std::convertible_to<T> U>

	Reference& operator=(Reference<U> rhs) noexcept {
		reference = rhs.ptr();
		return *this;
	};
	template<std::convertible_to<T> U>
		Reference& operator=(U* const rhs) noexcept {
		reference = rhs;
		return *this;
	};
	template<std::convertible_to<T> U>
	Reference& operator=(const std::unique_ptr<U>& rhs) noexcept {
		reference = rhs.get();
		return *this;
	};

public: // -------------------- その他関数 --------------------
	operator bool() const noexcept { return reference; };
	bool operator!() const noexcept { return !static_cast<bool>(*this); };
	T* operator->() const noexcept { return reference; };
	T* ptr() const noexcept { return reference; };

	/// <summary>
	/// 関連付けの削除
	/// </summary>
	void unref() noexcept { reference = nullptr; };

private:
	T* reference{ nullptr };
};

// -------------------- 等価演算子 --------------------

template<typename T, typename U>
bool operator==(const Reference<T>& lhs, const Reference<U>& rhs) noexcept { return lhs.ptr() == rhs.ptr(); };
template<typename T, typename U>
bool operator!=(const Reference<T>& lhs, const Reference<U>& rhs) noexcept { return !(lhs == rhs); };


// -------------------- 比較演算子 --------------------

template<typename T, typename U>
bool operator<(const Reference<T>& lhs, const Reference<U>& rhs) noexcept { return lhs.ptr() < rhs.ptr(); };
template<typename T, typename U>
bool operator>(const Reference<T>& lhs, const Reference<U>& rhs) noexcept { return rhs < lhs; };
template<typename T, typename U>
bool operator<=(const Reference<T>& lhs, const Reference<U>& rhs) noexcept { return !(rhs > lhs); };
template<typename T, typename U>
bool operator>=(const Reference<T>& lhs, const Reference<U>& rhs) noexcept { return !(rhs < lhs); };

template<typename T>
struct std::hash<Reference<T>> {
	uint64_t operator()(const Reference<T>& val) const {
		return std::hash<uint64_t>()(val);
	}
};
