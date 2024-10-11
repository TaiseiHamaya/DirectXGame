#pragma once

#include <format>
#include <filesystem>
#include <source_location>

template<typename Char>
struct FormatWithLocation {
	std::source_location location;
	std::basic_string_view<Char> str_;

	template<typename String>
		requires std::constructible_from<std::basic_string_view<Char>, String>
	consteval FormatWithLocation(
		String&& string,
		const std::source_location sourceLocation_ = std::source_location::current()
	) noexcept
		: str_(std::forward<String>(string)), location(sourceLocation_) {
	}
};

void ConsoleA(const std::string& msg);

void ConsoleW(const std::wstring& msg);

template<typename ...Args>
inline void Console(const FormatWithLocation<char>& msg, Args&& ...args) {
	std::filesystem::path path{ msg.location.file_name() };
	ConsoleA(std::format("[{}] ", path.stem().string()));
	std::string message = std::vformat(msg.str_, std::make_format_args(args ...));
	ConsoleA(message);
}

template<typename ...Args>
inline void Console(const FormatWithLocation<wchar_t>& msg, Args && ...args) {
	std::filesystem::path path{ msg.location.file_name() };
	ConsoleA(std::format("[{}] ", path.stem().string()));
	std::wstring message = std::vformat(msg.str_, std::make_wformat_args(args ...));
	ConsoleW(message);
}