#include "WinApp.h"

#include <cassert>
#include "Util.h"
#include <Engine/Core/DirectXCore.h>

#ifdef _DEBUG
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#include "Debug/ImGUIManager.h"
#endif // _DEBUG

WinApp* WinApp::instance = nullptr;

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

WinApp::WinApp(int32_t width, int32_t hight) : 
	kClientWidht(width),
	kClientHight(hight),
	hWnd(nullptr), 
	hInstance(nullptr) {
	msg = {};
}

void WinApp::Initialize(const std::string& programName, int32_t width, int32_t hight) {
	assert(!instance);
	instance = new WinApp{ width, hight };
	CoInitializeEx(0, COINIT_MULTITHREADED);
	instance->init_app(programName);
	//DirectXの初期化
	DirectXCore::Initialize(instance->hWnd);
	// ウィンドウ表示
	ShowWindow(instance->hWnd, SW_SHOW);
	Log("Complite Create Window\n");
}

bool WinApp::IsEndApp() {	// プロセスメッセージ取得用
	if (instance->msg.message == WM_QUIT) {
		return true;
	}

	// windowの×ボタンが押されるまでループ
	while (true) {
		// windowにメッセージが来たら最優先で処理
		if (PeekMessage(&instance->msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&instance->msg);
			DispatchMessage(&instance->msg);
		}
		else {
			return false;
		}
	}
}

void WinApp::BeginFrame() {
	instance->begin_frame();
}

void WinApp::EndFrame() {
	instance->end_frame();
}

void WinApp::Finalize() {
	instance->term_app();
	//windowを閉じる
	CloseWindow(instance->hWnd);
	//DirectXを終了
	DirectXCore::Finalize();
	// imguiの終了
	CoUninitialize();
	// 終了通知
	Log("End Program\n");
	delete instance;
}

void WinApp::init_app(const std::string& programName) {
	WNDCLASS wc{}; // ウィンドウの設定
	wc.lpfnWndProc = WindowProc;// ウィンドウプロシージャ
	auto&& name = ConvertString(programName);
	wc.lpszClassName = name.c_str();
	wc.hInstance = GetModuleHandle(nullptr);// インスタンスハンドル
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	// ウィンドウを登録
	RegisterClass(&wc);

	// ウィンドウサイズ指定用に構造体にする
	RECT wrc = { 0,0,kClientWidht, kClientHight };
	// 実際にwrcを変更
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの生成
	hWnd = CreateWindow(
		wc.lpszClassName,
		ConvertString(programName).c_str(),
		WS_OVERLAPPEDWINDOW,
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
	//D3D12Kernel::BeginFrame();
}

void WinApp::end_frame() {
	//D3D12Kernel::EndFrame();
}

void WinApp::term_app() {
	assert(instance);
}
