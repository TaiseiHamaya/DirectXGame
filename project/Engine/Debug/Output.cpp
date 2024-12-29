#include "Output.h"

#define NOMINMAX

#include <Windows.h>

#include <fstream>
#include <chrono>
#include <filesystem>
#include <format>

#include <Engine/Utility/Tools/ConvertString.h>

namespace chrono = std::chrono;

LocalTimeSeconds NowLocalSecond() {
	static const chrono::time_zone* timezone{ chrono::current_zone() };
	chrono::zoned_time nowZT{ timezone, chrono::system_clock::now()};
	LocalTimeSeconds nowZtFloor
		= chrono::floor<chrono::seconds>(nowZT.get_local_time());
	return nowZtFloor;
}

static const std::string LogFile{
	std::format("./Log/{:%F-%H%M%S}.log",
		NowLocalSecond())
};

void InitializeLog() {
	std::filesystem::path path{ LogFile };
	std::filesystem::create_directory(path.parent_path());
	std::ofstream file;
	file.exceptions(std::ios_base::badbit | std::ios_base::failbit);
	file.open(LogFile);
}

void ConsoleA(const std::string& msg) {
	std::ofstream outputFile{ LogFile, std::ios_base::app };
	outputFile.exceptions(std::ios_base::badbit | std::ios_base::failbit);
	outputFile << msg;
	outputFile.close();
	OutputDebugStringA(msg.c_str());
}

void ConsoleW(const std::wstring& msg) {
	std::ofstream outputFile{ LogFile, std::ios_base::app };
	outputFile.exceptions(std::ios_base::badbit | std::ios_base::failbit);
	outputFile << ConvertString(msg);
	outputFile.close();
	OutputDebugStringW(msg.c_str());
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
