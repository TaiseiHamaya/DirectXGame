#pragma once

// コピー禁止クラスマクロ
#define __UNCOPYABLE_CLASS(CLASS_NAME) \
public:\
CLASS_NAME() = default;\
~CLASS_NAME() = default;\
\
CLASS_NAME(const CLASS_NAME&) = delete;\
CLASS_NAME& operator=(const CLASS_NAME&&) = delete;\
CLASS_NAME(CLASS_NAME&&) = default;\
CLASS_NAME& operator=(CLASS_NAME&&) = default;

// コピー禁止クラスマクロ(ポリモーフィズム版)
#define __UNCOPYABLE_VIRTUAL_CLASS(CLASS_NAME) \
public:\
CLASS_NAME() = default;\
virtual ~CLASS_NAME() = default;\
\
CLASS_NAME(const CLASS_NAME&) = delete;\
CLASS_NAME& operator=(const CLASS_NAME&&) = delete;\
CLASS_NAME(CLASS_NAME&&) = default;\
CLASS_NAME& operator=(CLASS_NAME&&) = default;

// コピームーブ禁止クラスマクロ
#define __UNCOPYMOVEABLE_CLASS(CLASS_NAME) \
public:\
CLASS_NAME() = default;\
~CLASS_NAME() = default;\
\
CLASS_NAME(const CLASS_NAME&) = delete;\
CLASS_NAME& operator=(const CLASS_NAME&&) = delete;\
CLASS_NAME(CLASS_NAME&&) = delete;\
CLASS_NAME& operator=(CLASS_NAME&&) = delete;

// コピームーブ禁止クラスマクロ(ポリモーフィズム版)
#define __UNCOPYMOVEABLE_VIRTUAL_CLASS(CLASS_NAME) \
public:\
CLASS_NAME() = default;\
virtual ~CLASS_NAME() = default;\
\
CLASS_NAME(const CLASS_NAME&) = delete;\
CLASS_NAME& operator=(const CLASS_NAME&&) = delete;\
CLASS_NAME(CLASS_NAME&&) = delete;\
CLASS_NAME& operator=(CLASS_NAME&&) = delete;
