#include "Output.h"

#define NOMINMAX

#include <Windows.h>

void ConsoleA(const std::string& msg) {
	OutputDebugStringA(msg.c_str());
}

void ConsoleW(const std::wstring& msg) {
	OutputDebugStringW(msg.c_str());
}
