#pragma once

#include <string>
#include <memory>

std::wstring ConvertString(const std::string& str);
std::string ConvertString(const std::wstring& str);
void Log(const std::string& msg);
void Log(const std::wstring& msg);

template<class T, class... Args>
concept ConstructibleFrom = requires(Args&&... args) {
	T(std::forward<Args>(args)...);
};

template<class BaseType, class ...Args>
	requires ConstructibleFrom<BaseType, Args...>
std::unique_ptr<BaseType> CreateUnique(Args ...args) {
	return std::make_unique<BaseType>(args...);
}

template<class BaseType, class ...Args>
	requires ConstructibleFrom<BaseType, Args...>
std::shared_ptr<BaseType> CreateShared(Args ...args) {
	return std::shared_ptr<BaseType>(args...);
}
