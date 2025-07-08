#pragma once

// コピー禁止クラスマクロ
#define __CLASS_DEFAULT_ALL(...) \
public:\
	__VA_ARGS__(const __VA_ARGS__&) = default;\
	__VA_ARGS__& operator=(const __VA_ARGS__&) = default;\
	__VA_ARGS__(__VA_ARGS__&&) noexcept = default;\
	__VA_ARGS__& operator=(__VA_ARGS__&&) noexcept = default;

// コピー禁止クラスマクロ
#define __CLASS_NON_COPYABLE(...) \
public:\
	__VA_ARGS__(const __VA_ARGS__&) = delete;\
	__VA_ARGS__& operator=(const __VA_ARGS__&) = delete;\
	__VA_ARGS__(__VA_ARGS__&&) noexcept = default;\
	__VA_ARGS__& operator=(__VA_ARGS__&&) noexcept = default;

// コピー/ムーブ禁止クラスマクロ
#define __CLASS_NON_COPYMOVEABLE(...) \
public:\
	__VA_ARGS__(const __VA_ARGS__&) = delete;\
	__VA_ARGS__& operator=(const __VA_ARGS__&) = delete;\
	__VA_ARGS__(__VA_ARGS__&&) = delete;\
	__VA_ARGS__& operator=(__VA_ARGS__&&) = delete;
