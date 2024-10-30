#pragma once

#include <format>

void InitializeLog();

void ConsoleA(const std::string& msg);

void ConsoleW(const std::wstring& msg);

template<typename ...Args>
inline void Console(std::format_string<Args...> msg, Args && ...args) {
	std::string message = std::format(msg, std::forward<Args>(args)...);
	ConsoleA(message.c_str());
}

template<typename ...Args>
inline void Console(std::wformat_string<Args...> msg, Args && ...args) {
	std::wstring message = std::format(msg, std::forward<Args>(args)...);
	ConsoleW(message);
}
