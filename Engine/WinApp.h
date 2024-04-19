#pragma once

#include <cstdint>
#include <string>
#include <format>
#include <Windows.h>

class WinApp {
private:
	WinApp(int32_t width, int32_t hight);
	~WinApp() = default;

public:
	WinApp(const WinApp&) = delete;
	WinApp& operator=(const WinApp&) = delete;

public:
	static void Initialize(const std::string& programName, int32_t width, int32_t hight);
	static bool IsEndApp();
	static void BeginFrame();
	static void EndFrame();
	static void Finalize();

public:
	static int32_t GetClientWidth() { return instance->kClientWidht; };
	static int32_t GetClientHight() { return instance->kClientHight; };

private:
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <returns></returns>
	void init_app(const std::string& programName);
	void begin_frame();
	void end_frame();
	void term_app();

private:
	static WinApp* instance;

private:
	int32_t kClientWidht;
	int32_t kClientHight;

	HWND hWnd;
	HINSTANCE hInstance;

	MSG msg;
};

