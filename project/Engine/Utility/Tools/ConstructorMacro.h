#pragma once

// コピー禁止クラスマクロ
#define __NON_COPYABLE_CLASS(CLASS_NAME) \
public:\
	CLASS_NAME(const CLASS_NAME&) = delete;\
	CLASS_NAME& operator=(const CLASS_NAME&) = delete;\
	CLASS_NAME(CLASS_NAME&&) = default;\
	CLASS_NAME& operator=(CLASS_NAME&&) = default;

// コピー/ムーブ禁止クラスマクロ
#define __NON_COPYMOVEABLE_CLASS(CLASS_NAME) \
public:\
	CLASS_NAME(const CLASS_NAME&) = delete;\
	CLASS_NAME& operator=(const CLASS_NAME&) = delete;\
	CLASS_NAME(CLASS_NAME&&) = delete;\
	CLASS_NAME& operator=(CLASS_NAME&&) = delete;
