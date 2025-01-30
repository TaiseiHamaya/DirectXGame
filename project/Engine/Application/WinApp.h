#pragma once

#include <memory>

#include <windows.h>

class WinApp final {
private:
	WinApp() noexcept;

public:
	~WinApp() noexcept = default;

public:
	WinApp(const WinApp&) = delete;
	WinApp& operator=(const WinApp&) = delete;

public:
	static void Initialize(DWORD windowConfig = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
	static void BeginFrame();
	static void EndFrame();
	static void Finalize();

	static void ShowAppWindow();
	static bool IsEndApp();
	static void ProcessMessage();

public:
	static HWND& GetWndHandle() noexcept { return instance->hWnd; };
	static const HINSTANCE& GetWindowHandle() noexcept { return instance->hInstance; };

private:
	void initialize_application(DWORD windowConfig);

	void wait_frame();

private:
	static inline std::unique_ptr<WinApp> instance = nullptr;

private:

	HWND hWnd;
	HINSTANCE hInstance;

	MSG msg;
};

