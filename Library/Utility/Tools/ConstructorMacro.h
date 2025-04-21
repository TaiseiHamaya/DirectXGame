#pragma once

// コピー禁止クラスマクロ
#define __CLASS_DEFAULT_ALL(CLASS_NAME) \
public:\
	CLASS_NAME(const CLASS_NAME&) = default;\
	CLASS_NAME& operator=(const CLASS_NAME&) = default;\
	CLASS_NAME(CLASS_NAME&&) noexcept = default;\
	CLASS_NAME& operator=(CLASS_NAME&&) noexcept = default;

// コピー禁止クラスマクロ
#define __CLASS_NON_COPYABLE(CLASS_NAME) \
public:\
	CLASS_NAME(const CLASS_NAME&) = delete;\
	CLASS_NAME& operator=(const CLASS_NAME&) = delete;\
	CLASS_NAME(CLASS_NAME&&) noexcept = default;\
	CLASS_NAME& operator=(CLASS_NAME&&) noexcept = default;

// コピー/ムーブ禁止クラスマクロ
#define __CLASS_NON_COPYMOVEABLE(CLASS_NAME) \
public:\
	CLASS_NAME(const CLASS_NAME&) = delete;\
	CLASS_NAME& operator=(const CLASS_NAME&) = delete;\
	CLASS_NAME(CLASS_NAME&&) = delete;\
	CLASS_NAME& operator=(CLASS_NAME&&) = delete;
