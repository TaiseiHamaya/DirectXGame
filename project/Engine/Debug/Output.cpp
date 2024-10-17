#include "Output.h"

#define NOMINMAX

#include <Windows.h>

#include <fstream>
#include <chrono>
#include <filesystem>
#include <format>

#include <Engine/Utility/ConvertString.h>

static const std::string LogFile{
	std::format("./Log/{:%F-%H%M%S}.log",
		std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()))
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
