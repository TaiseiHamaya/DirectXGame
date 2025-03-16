#include "Output.h"

#include <Windows.h>

#include <dbghelp.h>

#include <array>
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <mutex>
#include <vector>

#include <Library/Utility/Tools/ConvertString.h>

#include "./WinApp.h"
#include "Engine/Application/EngineSettings.h"

namespace chrono = std::chrono;

std::mutex OutputMutex;

LocalTimeSeconds NowLocalSecond() {
	static const chrono::time_zone* timezone{ chrono::current_zone() };
	chrono::zoned_time nowZT{ timezone, chrono::system_clock::now() };
	LocalTimeSeconds nowZtFloor
		= chrono::floor<chrono::seconds>(nowZT.get_local_time());
	return nowZtFloor;
}

static const std::string LogFile{
	std::format("./Log/{:%F-%H%M%S}.log", NowLocalSecond())
};

constexpr std::array<const wchar_t*, 4> TypeStringW = {
	L"Infomation",
	L"Warning",
	L"Error",
	L"Critical",
};

void InitializeLog() {
	std::filesystem::path path{ LogFile };
	std::filesystem::create_directory(path.parent_path());
	std::ofstream file;
	file.exceptions(std::ios_base::badbit | std::ios_base::failbit);
	file.open(LogFile);
}

static void LogOutputFile(const std::string& msg) {
	std::ofstream outputFile{ LogFile, std::ios_base::app };
	outputFile << msg;
	outputFile.close();
}

static void LogOutputStacktrace() {
	std::array<void*, 100> stacks{};
	WORD numberOfFrames;
	HANDLE process = WinApp::GetProcessHandle();

	numberOfFrames = RtlCaptureStackBackTrace(0, 100, stacks.data(), nullptr);

	std::vector<char> symbolBuffer(sizeof(SYMBOL_INFO) + 256);
	SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(symbolBuffer.data());

	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	LogOutputFile("\nOutput stack trace.\n");
	OutputDebugStringA("\nOutput stack trace.\n");

	constexpr int NumSkipTrace{ 4 };
	for (int i = NumSkipTrace; i < numberOfFrames; i++) {
		SymFromAddr(process, reinterpret_cast<DWORD64>(stacks[i]), 0, symbol);
		std::string output = std::format("{: >2} | 0x{:016x} | {}\n", i - NumSkipTrace, symbol->Address, symbol->Name);
		LogOutputFile(output);
		OutputDebugStringA(output.c_str());
	}

	LogOutputFile("\n");
	OutputDebugStringA("\n");
}

static void LogWindow(LogType type, const std::wstring& caption, const std::wstring& msg) {
	UINT flag = 0;
	switch (type) {
	case LogType::Infomation:
		flag = MB_ICONINFORMATION | MB_OK;
		break;
	case LogType::Warning:
		flag = MB_ICONWARNING | MB_OK;
		break;
	case LogType::Error:
		flag = MB_ICONERROR | MB_OK;
		break;
	case LogType::Critical:
		flag = MB_ICONERROR | MB_OK;
		break;
	}
	MessageBoxW(nullptr, msg.c_str(), caption.c_str(), flag);
}

std::string_view ToFilenameA(const std::source_location& sourceLocation) {
	std::string_view fullPath = sourceLocation.file_name();
	size_t position = fullPath.find_last_of('\\') + 1;
	size_t end = fullPath.find_last_of('.');
	return fullPath.substr(position, end - position);
}

std::wstring ToFilenameW(const std::source_location& sourceLocation) {
	std::wstring fullPath = ConvertString(sourceLocation.file_name());
	size_t position = fullPath.find_last_of('\\') + 1;
	size_t end = fullPath.find_last_of('.');
	return fullPath.substr(position, end - position);
}

uint8_t GetConfigFlags(LogType type) {
	return (EngineSettings::LogOutputConfigFlags >> (static_cast<uint8_t>(type) * 6)) & 0b111111;
}

static void LogOutputBody(const LocalTimeSeconds& time, const std::wstring& file, LogType type, const std::wstring& message) {
	std::lock_guard<std::mutex> lock{ OutputMutex };

	uint8_t config = GetConfigFlags(type);
	std::wstring typeW = TypeStringW[static_cast<uint8_t>(type)];
	std::wstring out =
		std::format(L"{:%H:%M:%S} | {: <10} | {} [{}]\n", time, typeW, message, file);

	// コンソール出力
	if (config & 0b100000) {
		OutputDebugStringW(out.c_str());
	}
	// ファイル出力
	if (config & 0b010000) {
		LogOutputFile(ConvertString(out));
	}
	// Stacktraceの出力
	if (config & 0b000001) {
		LogOutputStacktrace();
	}
	// Editor出力
	if (config & 0b00100) {

	}
	// ウィンドウ出力
	if (config & 0b000100) {
		LogWindow(type, std::format(L"[{}] {}", typeW, file), message);
	}
	// ブレークポイントによって停止させる
	if (config & 0b000010) {
		__debugbreak();
	}
}

void LogOutputA(const LocalTimeSeconds& time, const std::wstring& file, LogType type, const std::string& message) {
	LogOutputBody(time, file, type, ConvertString(message));
}

void LogOutputW(const LocalTimeSeconds& time, const std::wstring& file, LogType type, const std::wstring& message) {
	LogOutputBody(time, file, type, message);
}
