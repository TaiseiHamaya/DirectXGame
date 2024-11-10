#pragma once

template <typename T>
class SingletonInterface {
protected:
	SingletonInterface() = default;
	~SingletonInterface() = default;

public:
	SingletonInterface(const SingletonInterface&) = delete;
	SingletonInterface& operator=(const SingletonInterface&&) = delete;

public:
	static T& GetInstance() {
		static T instance;
		return instance;
	}
};

/// SingletonInterfaceを使用するクラスでのコンストラクタマクロ
#define __SINGLETON_INTERFACE(CLASS_NAME)\
	friend class SingletonInterface<CLASS_NAME>;\
private:\
	CLASS_NAME() = default;\
	~CLASS_NAME() = default;\
\
public:\
	CLASS_NAME(const CLASS_NAME&) = delete;\
	CLASS_NAME& operator=(const CLASS_NAME&) = delete;\
	CLASS_NAME(CLASS_NAME&&) = delete;\
	CLASS_NAME& operator=(CLASS_NAME&&) = delete;

