#pragma once

#define NOMINMAX

#include <Windows.h>

#include <format>
#include <memory>

std::wstring ConvertString(const std::string& str);
std::string ConvertString(const std::wstring& str);

template<typename ...Args>
inline void Log(std::format_string<Args...> msg, Args && ...args) {
	std::string message = std::format(msg, std::forward<Args>(args)...);
	OutputDebugStringA(message.c_str());
}

template<typename ...Args>
inline void Log(std::wformat_string<Args...> msg, Args && ...args) {
	std::string message = ConvertString(std::format(msg, std::forward<Args>(args)...));
	OutputDebugStringA(message.c_str());
}

namespace eps {
template<class ConstructT, class... Args>
concept ConstructibleFrom = requires(Args&&... args) {
	ConstructT(std::forward<Args>(args)...); // ArgsでTが初期化可能
};

template<class BaseType, class ...Args>
	requires ConstructibleFrom<BaseType, Args...>
std::unique_ptr<BaseType> CreateUnique(Args ...args) {
	return std::make_unique<BaseType>(args...);
}

template<class BaseType, class ...Args>
	requires ConstructibleFrom<BaseType, Args...>
std::shared_ptr<BaseType> CreateShared(Args ...args) {
	return std::make_shared<BaseType>(args...);
}
}
