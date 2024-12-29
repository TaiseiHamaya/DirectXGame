#include "WinApp.h"

#include <cassert>

#include "Engine/Debug/Output.h"
#include "Engine/Rendering/DirectX/DirectXCore.h"
#include "Engine/Resources/Audio/AudioManager.h"
#include "Engine/Resources/BackgroundLoader/BackgroundLoader.h"
#include "Engine/Resources/PolygonMesh/PolygonMeshManager.h"
#include "Engine/Resources/Texture/TextureManager.h"
#include "Engine/Runtime/Input/Input.h"
#include "Engine/Runtime/Scene/SceneManager.h"
#include "Engine/Runtime/WorldClock/WorldClock.h"
#include "Engine/Utility/Tools/RandomEngine.h"
#include "EngineSettings.h"

#pragma comment(lib, "winmm.lib")

#ifdef _DEBUG
#include "Engine/Debug/ImGui/ImGuiManager/ImGuiManager.h"

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

WinApp::WinApp() noexcept :
	hWnd(nullptr),
	hInstance(nullptr) {
	msg = {};
}

void WinApp::Initialize(DWORD windowConfig) {
#ifdef _DEBUG
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG
	// chrono時間精度の設定
	timeBeginPeriod(1);

	assert(!instance);
	instance.reset(new WinApp{});
	// COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	InitializeLog();

	instance->initialize_application(windowConfig);
	//DirectXの初期化
	DirectXCore::Initialize();
	// テクスチャマネージャの初期化
	TextureManager::Initialize();
	// 音関連の初期化
	AudioManager::Initialize();
	// 入力の初期化
	Input::Initialize();
	// 乱数エンジンの初期化
	RandomEngine::Initialize();
	// 時計初期化
	WorldClock::Initialize();
	// バックグラウンドローダーの初期化
	BackgroundLoader::Initialize();

#ifdef _DEBUG
	ImGuiManager::Initialize();
#endif // _DEBUG
	// システム使用のオブジェクトをロード
	PolygonMeshManager::RegisterLoadQue("./EngineResources/Models/ErrorObject/ErrorObject.obj");
	PolygonMeshManager::RegisterLoadQue("./EngineResources/Models/Frustum/Frustum.obj");
	PolygonMeshManager::RegisterLoadQue("./EngineResources/Models/Grid/Grid.obj");
	PolygonMeshManager::RegisterLoadQue("./EngineResources/Models/Camera/CameraAxis.obj");

	// 待機
	BackgroundLoader::WaitEndExecute();

	Console("Complete initialize application.\n");
}

void WinApp::BeginFrame() {
	WorldClock::Update();
	Input::Update();
	DirectXCore::BeginFrame();
#ifdef _DEBUG
	ImGuiManager::BeginFrame();
#endif // _DEBUG
}

void WinApp::EndFrame() {
#ifdef _DEBUG
	// 一番先にImGUIの処理
	ImGuiManager::EndFrame();
#endif // _DEBUG

	DirectXCore::EndFrame();

	//instance->wait_frame();
}

void WinApp::Finalize() {
	// 終了通知
	Console("End Program.\n");
	//windowを閉じる
	CloseWindow(instance->hWnd);
	Console("Closed Window.\n");

	// 各種終了処理
	// Initializeと逆順でやる
	// シーン
	SceneManager::Finalize();
#ifdef _DEBUG
	// ImGui
	ImGuiManager::Finalize();
#endif // _DEBUG

	AudioManager::Finalize();

	TextureManager::Finalize();
	//DirectXを終了
	DirectXCore::Finalize();

	// COMの終了
	CoUninitialize();
	instance.reset();

	// ログ
	Console("Complete finalize application.\n");

	// chrono内のTZDBを削除
	std::chrono::get_tzdb_list().~tzdb_list();
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

void WinApp::initialize_application(DWORD windowConfig) {
	// ウィンドウの設定

	WNDCLASS windowClass{};
	windowClass.lpfnWndProc = WindowProc;// ウィンドウプロシージャ
	windowClass.lpszClassName = EngineSettings::WINDOW_TITLE_W.data();
	windowClass.hInstance = GetModuleHandle(nullptr); // インスタンスハンドル
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);

	// ウィンドウを登録
	RegisterClass(&windowClass);

	// ウィンドウサイズ指定用に構造体にする
	RECT wrc = { 0,0,
		EngineSettings::CLIENT_WIDTH, EngineSettings::CLIENT_HEIGHT };
	// 実際にwrcを変更
	AdjustWindowRect(&wrc, windowConfig, false);

	// ウィンドウの生成
	hWnd = CreateWindowW(
		windowClass.lpszClassName,
		EngineSettings::WINDOW_TITLE_W.data(),
		windowConfig,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		windowClass.hInstance,
		nullptr
	);

	hInstance = windowClass.hInstance;
}

#include <thread>

void WinApp::wait_frame() {
	using millisecond_f = std::chrono::duration<float, std::milli>;

	//constexpr millisecond_f MinTime{ 1000.00000f / 60.0f };
	constexpr millisecond_f MinCheckTime{ 1000.00000f / 65.0f }; // 少し短い時間を使用することで60FPSになるようにする
	// 開始
	auto& begin = WorldClock::BeginTime();
	// 今
	auto now = std::chrono::high_resolution_clock::now();
	// 経過時間
	auto duration = std::chrono::duration_cast<millisecond_f>(now - begin);
	// 基準より短い場合
	if (duration < MinCheckTime) {
		// 残りの止める時間
		millisecond_f sleepMilliSec = MinCheckTime - duration;
		// 止める
		std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::microseconds>(sleepMilliSec));
	}
}
