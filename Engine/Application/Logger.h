#pragma once

#include <array>
#include <format>
#include <fstream>
#include <mutex>
#include <source_location>
#include <string>

#include <Library/Utility/Template/bitflag.h>
#include <Library/Utility/Template/SingletonInterface.h>

class Logger : public SingletonInterface<Logger> {
	__CLASS_SINGLETON_INTERFACE(Logger)

public:
	enum class Type : u8 {
		Trace,
		Information,
		Warning,
		Error,
		Critical,
		Assert,
	};

	enum class OutputDestination : u8 {
		Console = 1 << 0,
		File = 1 << 1,
		Editor = 1 << 2,
		Window = 1 << 3,
		StackTrace = 1 << 4,
		Breakpoint = 1 << 5,
	};

public:
	static void Initialize();

	static void Finalize();

	static void SyncErrorWindow();

	template<typename ...Args>
	static void LogEntryPoint(const std::source_location& sourceLocation, Type type, std::format_string<Args...> msg, Args&& ...args);

	template<typename ...Args>
	static void LogEntryPoint(const std::source_location& sourceLocation, Type type, std::wformat_string<Args...> msg, Args&& ...args);

private:
	void body(const std::wstring& file, Type type, const std::wstring& message);

	void intermediate_a(const std::source_location& sourceLocation, Type type, const std::string& message);
	void intermediate_w(const std::source_location& sourceLocation, Type type, const std::wstring& message);

	void output_console(const std::wstring& msg);
	void output_file(const std::wstring& msg);
	void popup_window(Type type, const std::wstring& caption, const std::wstring& msg);

	void stack_trace();
	void stack_trace_line(const std::wstring& msg);

private:
	bool canOutputLog{ false };
	std::mutex mutex;
	std::wofstream logFile;

private:
	static constexpr std::array<wstring_literal, 8> TypeStringW = {
		L"Trace",
		L"Information",
		L"Warning",
		L"Error",
		L"Critical",
		L"Assert",
		L"",
		L"",
	};
};

__USE_BITFLAG(Logger::OutputDestination)

template<typename ...Args>
inline void Logger::LogEntryPoint(const std::source_location& sourceLocation, Type type, std::format_string<Args...> msg, Args && ...args) {
	std::string message = std::format(msg, std::forward<Args>(args)...);
	Logger::GetInstance().intermediate_a(sourceLocation, type, message);
}

template<typename ...Args>
inline void Logger::LogEntryPoint(const std::source_location& sourceLocation, Type type, std::wformat_string<Args...> msg, Args && ...args) {
	std::wstring message = std::format(msg, std::forward<Args>(args)...);
	Logger::GetInstance().intermediate_w(sourceLocation, type, message);
}

#define szgTrace(msg, ...) Logger::LogEntryPoint(std::source_location::current(), Logger::Type::Trace, msg, __VA_ARGS__)
#define szgInformation(msg, ...) Logger::LogEntryPoint(std::source_location::current(), Logger::Type::Information, msg, __VA_ARGS__)
#define szgWarning(msg, ...) Logger::LogEntryPoint(std::source_location::current(), Logger::Type::Warning, msg, __VA_ARGS__)
#define szgError(msg, ...) Logger::LogEntryPoint(std::source_location::current(), Logger::Type::Error, msg, __VA_ARGS__)
#define szgCritical(msg, ...) Logger::LogEntryPoint(std::source_location::current(), Logger::Type::Critical, msg, __VA_ARGS__)

#define szgWarningIf(conditional, msg, ...) if((conditional)) szgWarning(msg, __VA_ARGS__)
#define szgErrorIf(conditional, msg, ...) if((conditional)) szgError(msg, __VA_ARGS__)
#define szgCriticalIf(conditional, msg, ...) if((conditional)) szgCritical(msg, __VA_ARGS__)

#define szgAssert(conditional) if (!(conditional)) Logger::LogEntryPoint(std::source_location::current(), Logger::Type::Assert, L"Assertion failed: {}", L#conditional)
