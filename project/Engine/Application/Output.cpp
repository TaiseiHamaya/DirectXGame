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

#include <Library/Utility/Tools/ChronoUtility.h>
#include <Library/Utility/Tools/ConvertString.h>

#include "./WinApp.h"
#include "Engine/Application/EngineSettings.h"

namespace chrono = std::chrono;

std::mutex OutputMutex;

constexpr std::array<const wchar_t*, 4> TypeStringW = {
	L"Infomation",
	L"Warning",
	L"Error",
	L"Critical",
};

void InitializeLog() {
	std::filesystem::create_directory(EngineSettings::LogFilePath.parent_path());
	std::ofstream file;
	file.exceptions(std::ios_base::badbit | std::ios_base::failbit);
	file.open(EngineSettings::LogFilePath);
}

void SyncErrorWindow() {
	std::lock_guard<std::mutex> lock{ OutputMutex };
}

static void LogOutputFile(const std::wstring& msg) {
	std::wofstream outputFile{ EngineSettings::LogFilePath, std::ios_base::app };
	outputFile << msg;
	outputFile.close();
}

void StacktraceOutputBody(std::wstring msg) {
	LogOutputFile(msg);
	OutputDebugStringW(msg.c_str());
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

	StacktraceOutputBody(L"\nOutput stack trace.\n");

#ifdef _DEBUG
	constexpr int NumSkipTrace{ 4 };
#else
	constexpr int NumSkipTrace{ 2 };
#endif // _DEBUG

	for (int i = NumSkipTrace; i < numberOfFrames; i++) {
		SymFromAddr(process, reinterpret_cast<DWORD64>(stacks[i]), 0, symbol);
		std::wstring output = std::format(L"{: >2} | 0x{:016x} | {}\n", i - NumSkipTrace, symbol->Address, ConvertString(symbol->Name));
		StacktraceOutputBody(output);
	}

	StacktraceOutputBody(L"\n");
}

static void LogWindow(LogType type, const std::wstring& caption, const std::wstring& msg) {
	UINT flag = 0;
	switch (type) {
	case LogType::Infomation:
		flag = MB_ICONINFORMATION;
		break;
	case LogType::Warning:
		flag = MB_ICONWARNING;
		break;
	case LogType::Error:
		flag = MB_ICONERROR;
		break;
	case LogType::Critical:
		flag = MB_ICONERROR;
		break;
	}

	flag |= MB_OK | MB_SYSTEMMODAL;
	MessageBoxW(nullptr, msg.c_str(), caption.c_str(), flag);
}

std::string_view ToFilenameA(const std::source_location& sourceLocation) {
	std::string_view fullPath = sourceLocation.file_name();
	size_t position = fullPath.find_last_of('\\') + 1;
	size_t end = fullPath.find_last_of('.');
	return fullPath.substr(position, end - position);
}

std::wstring ToFilenameW(const std::source_location& sourceLocation) {
	std::string_view fullPath = sourceLocation.file_name();
	size_t position = fullPath.find_last_of('\\') + 1;
	size_t end = fullPath.find_last_of('.');
	return ConvertString(fullPath.substr(position, end - position));
}

uint8_t GetConfigFlags(LogType type) {
	return (EngineSettings::LogOutputConfigFlags >> (static_cast<uint8_t>(type) * 6)) & 0b111111;
}

static void LogOutputBody(const std::wstring& file, LogType type, const std::wstring& message) {
	std::lock_guard<std::mutex> lock{ OutputMutex };

	ChronoUtility::LocalTimeSeconds time = ChronoUtility::NowLocalSecond();

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
		LogOutputFile(out);
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
#ifdef DEBUG_FEATURES_ENABLE
		__debugbreak();
#else
		throw std::runtime_error("致命的なエラーが発生したためアプリケーションを停止しました。");
#endif // DEBUG_FEATURES_ENABLE
	}
}

void LogOutputA(const std::source_location& sourceLocation, LogType type, const std::string& message) {
	LogOutputBody(ToFilenameW(sourceLocation), type, ConvertString(message));
}

void LogOutputW(const std::source_location& sourceLocation, LogType type, const std::wstring& message) {
	LogOutputBody(ToFilenameW(sourceLocation), type, message);
}
