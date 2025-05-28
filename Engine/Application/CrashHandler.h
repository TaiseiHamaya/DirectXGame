#pragma once

#include <Windows.h>

class CrashHandler {
public:
	static void Initialize();

	static LONG __stdcall ExportDump(EXCEPTION_POINTERS* exception) noexcept;
};
