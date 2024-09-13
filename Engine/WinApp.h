#pragma once

#define NOMINMAX

#include <cstdint>
#include <string>
#include <windows.h>

class WinApp final {
private:
	WinApp(int32_t width, int32_t height) noexcept;
	~WinApp() noexcept = default;

public:
	WinApp(const WinApp&) = delete;
	WinApp& operator=(const WinApp&) = delete;

public:
	static void Initialize(const std::string& programName, int32_t width = 1280, int32_t height = 720, DWORD windowConfig = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
	static bool IsEndApp();
	static void BeginFrame();
	static void EndFrame();
	static void Finalize();

public:
	static int32_t GetClientWidth() noexcept { return instance->kClientWidth; };
	static int32_t GetClientHight() noexcept { return instance->kClientHight; };
	static HWND& GetWndHandle() noexcept { return instance->hWnd; };

private:
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <returns></returns>
	void init_app(const std::string& programName, DWORD windowConfig);
	void begin_frame();
	void end_frame();
	void term_app();

	void wait_frame();

private:
	static WinApp* instance;

private:
	int32_t kClientWidth;
	int32_t kClientHight;

	HWND hWnd;
	HINSTANCE hInstance;

	MSG msg;

	WNDCLASS wc{};

	std::string windowName;
};

