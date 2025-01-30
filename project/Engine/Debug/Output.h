#pragma once

#include <chrono>
#include <format>
#include <source_location>
#include <string_view>

using LocalTimeSeconds = std::chrono::time_point<std::chrono::local_t, std::chrono::seconds>;

LocalTimeSeconds NowLocalSecond();

void InitializeLog();

void ConsoleA(const std::string& msg);

void ConsoleW(const std::wstring& msg);

std::string_view ToFilenameA(const std::source_location& sourceLocation);
std::wstring ToFilenameW(const std::source_location& sourceLocation);

template<typename ...Args>
inline void ConsoleFormattingSL(const std::source_location& sourceLocation, std::format_string<Args...> msg, Args && ...args) {
	std::string_view file = ToFilenameA(sourceLocation);

	LocalTimeSeconds time = NowLocalSecond();

	std::string message = std::format(msg, std::forward<Args>(args)...);
	ConsoleA(std::format("{:%H:%M:%S} [{}] {}", time, file, message));
}

template<typename ...Args>
inline void ConsoleFormattingSL(const std::source_location& sourceLocation, std::wformat_string<Args...> msg, Args && ...args) {
	std::wstring file = ToFilenameW(sourceLocation);

	LocalTimeSeconds time = NowLocalSecond();

	std::wstring message = std::format(msg, std::forward<Args>(args)...);
	ConsoleW(std::format(L"{:%H:%M:%S} [{}] {}", time, file, message));
}

#define Console(msg, ...) ConsoleFormattingSL(std::source_location::current(), msg, __VA_ARGS__)
