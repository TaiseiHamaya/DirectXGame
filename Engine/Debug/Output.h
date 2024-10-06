#pragma once

#define NOMINMAX

#include <Windows.h>

#include <format>

#include "Engine/Utility/ConvertString.h"

template<typename ...Args>
inline void Console(std::format_string<Args...> msg, Args && ...args) {
	std::string message = std::format(msg, std::forward<Args>(args)...);
	OutputDebugStringA(message.c_str());
}

template<typename ...Args>
inline void Console(std::wformat_string<Args...> msg, Args && ...args) {
	std::wstring message = std::format(msg, std::forward<Args>(args)...);
	OutputDebugStringA(ConvertString(message).c_str());
}
