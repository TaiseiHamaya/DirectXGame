#pragma once

#include <memory>

#include <windows.h>

#include "Engine/Debug/Profiler/TimestampProfiler.h"

class WinApp final {
private:
	WinApp() noexcept;

public:
	~WinApp() noexcept;

public:
	WinApp(const WinApp&) = delete;
	WinApp& operator=(const WinApp&) = delete;

public:
	static void Initialize();
	static void BeginFrame();
	static void EndFrame();
	static void Finalize();

	static void ShowAppWindow();
	static bool IsEndApp();
	static void ProcessMessage();

public:
	static HWND GetWndHandle() noexcept { return instance->hWnd; };
	static HANDLE GetProcessHandle() noexcept { return instance->hProcess; };
	static HINSTANCE GetInstanceHandle() noexcept { return instance->hInstance; };

private:
	void initialize_application();

	void wait_frame();

private:
	static inline std::unique_ptr<WinApp> instance{ nullptr };

private:
	bool isEndApp{ false };
	HWND hWnd{ nullptr };
	HINSTANCE hInstance{ nullptr };
	HANDLE hProcess{ nullptr };

	MSG msg;

#ifdef DEBUG_FEATURES_ENABLE
public:
	static bool IsStopUpdate() { return instance->isStopUpdate && !instance->isPassedPause; }

private:
	bool isStopUpdate;
	bool isPassedPause;
	mutable TimestampProfiler profiler;
#endif // _DEBUG
};

