#pragma once

#include <memory>

#include <windows.h>

#include <Library/Utility/Template/SingletonInterface.h>

#include "Engine/Debug/Profiler/TimestampProfiler.h"

class WinApp final : public SingletonInterface<WinApp> {
	friend class SingletonInterface<WinApp>;

private:
	WinApp() = default;
	~WinApp();

public:
	WinApp(const WinApp&) = delete;
	WinApp& operator=(const WinApp&) = delete;
	WinApp(WinApp&&) = delete;
	WinApp& operator=(WinApp&&) = delete;

public:
	static void Initialize();
	
	static void BeginFrame();

	static void Update();

	static void Draw();

	static void EndFrame();
	
	static void Finalize();

public:
	static void ShowAppWindow();
	static bool IsEndApp();
	static void ProcessMessage();

public:
	static HWND GetWndHandle() noexcept { return GetInstance().hWnd; };
	static HANDLE GetProcessHandle() noexcept { return GetInstance().hProcess; };
	static HINSTANCE GetInstanceHandle() noexcept { return GetInstance().hInstance; };

private:
	void initialize_application();

	void wait_frame();

private:
	static inline bool isInitialized{ false };

private:
	bool isEndApp{ false };
	HWND hWnd{ nullptr };
	HINSTANCE hInstance{ nullptr };
	HANDLE hProcess{ nullptr };

	MSG msg{};

#ifdef DEBUG_FEATURES_ENABLE
public:
	static bool IsStopUpdate();

private:
	bool isStopUpdate;
	bool isPassedPause;
	mutable TimestampProfiler profiler;
#endif // _DEBUG
};

