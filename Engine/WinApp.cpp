#include "WinApp.h"

#include <cassert>
//#include <thread>

#include "Engine/Debug/Output.h"
#include "Engine/DirectX/DirectXCore.h"
#include "Engine/Application/Audio/AudioManager.h"
#include "Engine/Application/WorldClock/WorldClock.h"
#include "Engine/Application/Scene/SceneManager.h"
#include "Engine/Application/Input/Input.h"

#ifdef _DEBUG
#include <imgui.h>
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif // _DEBUG

// ウィンドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
#ifdef _DEBUG
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return true;
#endif // _DEBUG
	// メッセージによって変える
	switch (msg) {
	case WM_DESTROY: //ウィンドウが破壊されたとき
		PostQuitMessage(0); // OSに通知
		return 0;
	}
	// 通常時は通常のメッセージ処理
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

WinApp::WinApp(int32_t width, int32_t height) noexcept :
	kClientWidth(width),
	kClientHight(height),
	hWnd(nullptr),
	hInstance(nullptr) {
	msg = {};
}

void WinApp::Initialize(const std::string& programName, int32_t width, int32_t height, DWORD windowConfig) {
#ifdef _DEBUG
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG
	assert(!instance);
	instance.reset(new WinApp{ width, height });
	// COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);
	instance->init_app(programName, windowConfig);
	//DirectXの初期化
	DirectXCore::Initialize();

	AudioManager::Initialize();

	Input::Initialize();

	WorldClock::Initialize();

	Console("Complite initialize application.\n");
}

void WinApp::ShowAppWindow() {
	// ウィンドウ表示
	ShowWindow(instance->hWnd, SW_SHOW);
	Console("Show application window.\n");
}

bool WinApp::IsEndApp() {	// プロセスメッセージ取得用
	if (instance->msg.message == WM_QUIT) { // ×ボタンが押されたら終わる
		return true;
	}
	if (SceneManager::IsEndProgram()) {
		return true;
	}
	return false;
}

void WinApp::BeginFrame() {
	instance->begin_frame();
	WorldClock::Update();
	Input::Update();
}

void WinApp::EndFrame() {
	instance->end_frame();
}

void WinApp::Finalize() {
	// 終了通知
	Console("End Program.\n");
	//windowを閉じる
	CloseWindow(instance->hWnd);
	Console("Closed Window.\n");

	SceneManager::Finalize();

	AudioManager::Finalize();
	//DirectXを終了
	DirectXCore::Finalize();

	instance->term_app();
	// COMの終了
	CoUninitialize();
	instance.reset();
	// App
	Console("Complite finalize application.\n");
}

void WinApp::ProcessMessage() {
	// windowの×ボタンが押されるまでループ
	while (true) {
		// windowにメッセージが来たら最優先で処理
		if (PeekMessage(&instance->msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&instance->msg);
			DispatchMessage(&instance->msg);
		}
		else {
			return;
		}
	}
}

void WinApp::init_app(const std::string& programName, DWORD windowConfig) {
	windowName = programName;
	// ウィンドウの設定
	wc.lpfnWndProc = WindowProc;// ウィンドウプロシージャ
	auto&& name = ConvertString(programName);
	wc.lpszClassName = name.c_str();
	wc.hInstance = GetModuleHandle(nullptr); // インスタンスハンドル
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	// ウィンドウを登録
	RegisterClass(&wc);

	// ウィンドウサイズ指定用に構造体にする
	RECT wrc = { 0,0,kClientWidth, kClientHight };
	// 実際にwrcを変更
	AdjustWindowRect(&wrc, windowConfig, false);

	// ウィンドウの生成
	hWnd = CreateWindowW(
		wc.lpszClassName,
		name.c_str(),
		windowConfig,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);
}

void WinApp::begin_frame() {
	DirectXCore::BeginFrame();
}

void WinApp::end_frame() {
	DirectXCore::EndFrame();
	//wait_frame();
}

void WinApp::term_app() {
	assert(instance);
}

void WinApp::wait_frame() {
	using namespace std::literals::chrono_literals;
	using millisecond_f = std::chrono::duration<float, std::milli>;

	auto& begin = WorldClock::BeginTime();
	while (true) {
		auto now = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<millisecond_f>(now - begin);
		if (duration >= 16.66666ms) {
			return;
		}
	}
}
