#pragma once

// デフォルト定義マクロ
#define SZG_CLASS_DEFAULT(...) \
public:\
	__VA_ARGS__(const __VA_ARGS__&) = default;\
	__VA_ARGS__& operator=(const __VA_ARGS__&) = default;\
	__VA_ARGS__(__VA_ARGS__&&) noexcept = default;\
	__VA_ARGS__& operator=(__VA_ARGS__&&) noexcept = default;

// コピー禁止クラスマクロ
#define SZG_CLASS_MOVE_ONLY(...) \
public:\
	__VA_ARGS__(const __VA_ARGS__&) = delete;\
	__VA_ARGS__& operator=(const __VA_ARGS__&) = delete;\
	__VA_ARGS__(__VA_ARGS__&&) noexcept = default;\
	__VA_ARGS__& operator=(__VA_ARGS__&&) noexcept = default;

// コピー禁止クラスマクロ
#define SZG_CLASS_MOVE_ONLY_EXEPTION(...) \
public:\
	__VA_ARGS__(const __VA_ARGS__&) = delete;\
	__VA_ARGS__& operator=(const __VA_ARGS__&) = delete;\
	__VA_ARGS__(__VA_ARGS__&&) = default;\
	__VA_ARGS__& operator=(__VA_ARGS__&&) = default;

// コピー/ムーブ禁止クラスマクロ
#define SZG_CLASS_DELETED(...) \
public:\
	__VA_ARGS__(const __VA_ARGS__&) = delete;\
	__VA_ARGS__& operator=(const __VA_ARGS__&) = delete;\
	__VA_ARGS__(__VA_ARGS__&&) = delete;\
	__VA_ARGS__& operator=(__VA_ARGS__&&) = delete;
