#include "WinApp.h"

#include <dbghelp.h>
#include <timeapi.h>

#include <Library/Utility/Tools/ConvertString.h>
#include <Library/Utility/Tools/RandomEngine.h>

#include "Engine/Application/CrashHandler.h"
#include "Engine/Application/Output.h"
#include "Engine/Assets/Audio/AudioManager.h"
#include "Engine/Assets/BackgroundLoader/BackgroundLoader.h"
#include "Engine/Assets/PolygonMesh/PolygonMeshLibrary.h"
#include "Engine/Assets/PrimitiveGeometry/PrimitiveGeometryAsset.h"
#include "Engine/Assets/PrimitiveGeometry/PrimitiveGeometryLibrary.h"
#include "Engine/Assets/Shader/ShaderLibrary.h"
#include "Engine/Assets/Texture/TextureLibrary.h"
#include "Engine/GraphicsAPI/DirectX/DxCore.h"
#include "Engine/GraphicsAPI/DirectX/DxDescriptorHeap/SRVDescriptorHeap/SRVDescriptorHeap.h"
#include "Engine/Runtime/Clock/WorldClock.h"
#include "Engine/Runtime/Input/Input.h"
#include "Engine/Application/ProjectSettings/ProjectSettings.h"
#include "Engine/Runtime/Scene/SceneManager.h"

#ifdef DEBUG_FEATURES_ENABLE
#include "Engine/Debug/Editor/EditorMain.h"
#endif // DEBUG_FEATURES_ENABLE

#pragma comment(lib, "Dbghelp.lib") // Symとか
#pragma comment(lib, "Oleacc.lib") // GetProcessHandleFromHwnd
#pragma comment(lib, "winmm.lib") // timeBeginPeriod

extern "C" HANDLE WINAPI GetProcessHandleFromHwnd(_In_ HWND hwnd);

#ifdef DEBUG_FEATURES_ENABLE
#include "Engine/Debug/ImGui/ImGuiManager/ImGuiManager.h"

#include <imgui.h>
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif // _DEBUG
#include <Engine/Debug/Editor/Window/EditorLogWindow.h>

// ウィンドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
#ifdef DEBUG_FEATURES_ENABLE
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

WinApp::~WinApp() noexcept {
	// ログ
	Information("Complete finalize application.");
	FinalizeLog();
	// COMの終了
	CoUninitialize();
	// chrono内のTZDBを削除(これ以降ログ出力はされない)
	std::chrono::get_tzdb_list().~tzdb_list();
}

void WinApp::Initialize() {
	// ---------- ライブラリ、APIの初期化 ----------
#ifdef DEBUG_FEATURES_ENABLE
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

	ErrorIf(isInitialized, "WinApp is already initialized.");
	isInitialized = true;

	// アプリケーション内のwstring charsetをutf-8にする
	std::locale::global(std::locale("ja_JP.Utf-8"));

	// クラッシュハンドラの設定
	CrashHandler::Initialize();

	// COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	// Log出力システムの初期化
	InitializeLog();
#ifdef DEBUG_FEATURES_ENABLE
	EditorLogWindow::Allocate();
#endif // DEBUG_FEATURES_ENABLE

	// ---------- Projectのロード ----------
	ProjectSettings::Initialize();

	// ---------- WindowsApplicationの起動 ----------
	// アプリケーションの初期化
	auto& instance = GetInstance();
	instance.initialize_application();
	// シンボルハンドラーの初期化
	SymInitialize(instance.hProcess, nullptr, true);

	// ---------- エンジン機能の初期化 ----------
	//DirectXの初期化
	DxCore::Initialize();
	// テクスチャマネージャの初期化
	TextureLibrary::Initialize();
	// 音関連の初期化
	AudioManager::Initialize();
	// 入力の初期化
	Input::Initialize();
	// 乱数エンジンの初期化
	RandomEngine::Initialize();
	// バックグラウンドローダーの初期化
	BackgroundLoader::Initialize();

#ifdef DEBUG_FEATURES_ENABLE
	ImGuiManager::Initialize();
#endif // _DEBUG

	// システム使用のアセットをロード
	// メッシュ
	PolygonMeshLibrary::RegisterLoadQue("./DirectXGame/EngineResources/Models/ErrorObject/ErrorObject.obj");
	PolygonMeshLibrary::RegisterLoadQue("./DirectXGame/EngineResources/Models/Grid/Grid.obj");
	PolygonMeshLibrary::RegisterLoadQue("./DirectXGame/EngineResources/Models/Camera/CameraAxis.obj");
	// Primitive
	PrimitiveGeometryLibrary::Transfer(
		"Ico3",
		std::make_shared<PrimitiveGeometryAsset>("./DirectXGame/EngineResources/Json/PrimitiveGeometry/Lighting/Ico3.json")
	);
	PrimitiveGeometryLibrary::Transfer(
		"Rect3D",
		std::make_shared<PrimitiveGeometryAsset>("./DirectXGame/EngineResources/Json/PrimitiveGeometry/Primitive/Rect3D.json")
	);

#ifdef DEBUG_FEATURES_ENABLE
	// デバッグ用アセットのロード
	// Shader
	ShaderLibrary::RegisterLoadQue("./DirectXGame/EngineResources/HLSL/Misc/PrimitiveGeometry/PrimitiveGeometry.VS.hlsl");
	ShaderLibrary::RegisterLoadQue("./DirectXGame/EngineResources/HLSL/Misc/PrimitiveGeometry/PrimitiveGeometry.PS.hlsl");
	ShaderLibrary::RegisterLoadQue("./DirectXGame/EngineResources/HLSL/Forward/Mesh/StaticMeshForward.VS.hlsl");
	ShaderLibrary::RegisterLoadQue("./DirectXGame/EngineResources/HLSL/Forward/Forward.PS.hlsl");

	PrimitiveGeometryLibrary::Transfer(
		"SphereCollider",
		std::make_shared<PrimitiveGeometryAsset>("./DirectXGame/EngineResources/Json/PrimitiveGeometry/Collider/Sphere.json")
	);
	PrimitiveGeometryLibrary::Transfer(
		"AABBCollider",
		std::make_shared<PrimitiveGeometryAsset>("./DirectXGame/EngineResources/Json/PrimitiveGeometry/Collider/AABB.json")
	);
	PrimitiveGeometryLibrary::Transfer(
		"Frustum",
		std::make_shared<PrimitiveGeometryAsset>("./DirectXGame/EngineResources/Json/PrimitiveGeometry/Frustum.json")
	);
#endif // _DEBUG

	// 待機
	BackgroundLoader::WaitEndExecute();

	SceneManager::Initialize();

#ifdef DEBUG_FEATURES_ENABLE
	EditorMain::Initialize();
	SceneManager::SetProfiler(instance.profiler);
#endif // _DEBUG

	Information("Complete initialize application.");
}

void WinApp::BeginFrame() {
	SyncErrorWindow();

#ifdef DEBUG_FEATURES_ENABLE
	auto& instance = GetInstance();
	instance.profiler.clear_timestamps();
	instance.profiler.timestamp("BeginFrame");
#endif // _DEBUG
	WorldClock::Update();
	Input::Update();
	DxCore::BeginFrame();
#ifdef DEBUG_FEATURES_ENABLE
	ImGuiManager::BeginFrame();
	EditorMain::DrawBase();
#endif // _DEBUG
}

void WinApp::EndFrame() {
#ifdef DEBUG_FEATURES_ENABLE
	auto& instance = GetInstance();
	instance.profiler.timestamp("EndFrame");
	SceneManager::DebugGui();
	instance.profiler.timestamp("End");
	ImGui::Begin("Application");
	ImGui::Checkbox("IsStopUpdate", &instance.isStopUpdate);
	instance.isPassedPause = false;
	if (ImGui::Button("NextFrame")) instance.isPassedPause = true;
	ImGui::SeparatorText("Profiler");
	instance.profiler.debug_gui();
	ImGui::End();

	EditorMain::Draw();

	// 一番先にImGUIの処理
	ImGuiManager::EndFrame();
#endif // _DEBUG

	DxCore::EndFrame();

	//instance->wait_frame();
}

void WinApp::Finalize() {
	// 終了通知
	Information("End Program.");
	//windowを閉じる
	CloseWindow(GetInstance().hWnd);
	Information("Closed Window.");

	// 各種終了処理
	// Initializeと逆順でやる
	// シーン
	SceneManager::Finalize();
#ifdef DEBUG_FEATURES_ENABLE
	// ImGui
	EditorMain::Finalize();
	ImGuiManager::Finalize();
#endif // _DEBUG

	BackgroundLoader::Finalize();

	AudioManager::Finalize();

	TextureLibrary::Finalize();
	//DirectXを終了
	DxCore::Finalize();
}

void WinApp::ShowAppWindow() {
	// ウィンドウ表示
	ShowWindow(GetInstance().hWnd, SW_SHOW);
	Information("Show application window.");

#ifdef DEBUG_FEATURES_ENABLE
	EditorMain::Setup();
#endif // _DEBUG

	// 時計初期化
	WorldClock::Initialize();
}

bool WinApp::IsEndApp() {
	if (GetInstance().isEndApp) { // ×ボタンが押されたら終わる
		return true;
	}
	if (SceneManager::IsEndProgram()) {
		return true;
	}
	return false;
}

void WinApp::ProcessMessage() {
	auto& instance = GetInstance();
	while (true) {
		// windowにメッセージが来たら最優先で処理
		if (PeekMessage(&instance.msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&instance.msg);
			DispatchMessage(&instance.msg);
		}
		else {
			return;
		}
		switch (instance.msg.message) {
		case WM_QUIT: // windowの×ボタンが押されたら通知
			instance.isEndApp = true;
			break;
		}
	}
}

void WinApp::initialize_application() {
	std::wstring windowTitleW = ConvertString(ProjectSettings::GetProjectName());

	// ウィンドウの設定
	WNDCLASS windowClass{};
	windowClass.lpfnWndProc = WindowProc;// ウィンドウプロシージャ
	windowClass.lpszClassName = windowTitleW.data();
	windowClass.hInstance = GetModuleHandle(nullptr); // インスタンスハンドル
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);

	// ウィンドウを登録
	RegisterClass(&windowClass);

	// ウィンドウサイズ指定用に構造体にする
	RECT wrc = { 0,0,
		static_cast<LONG>(ProjectSettings::ClientWidth()), static_cast<LONG>(ProjectSettings::ClientHeight()) };
	// 実際にwrcを変更
	AdjustWindowRect(&wrc, ProjectSettings::WindowStyle(), false);

	// ウィンドウの生成
	hWnd = CreateWindowW(
		windowClass.lpszClassName,
		windowTitleW.data(),
		ProjectSettings::WindowStyle(),
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

	hProcess = GetProcessHandleFromHwnd(hWnd);
}

#include <thread>

void WinApp::wait_frame() {
	using millisecond_f = std::chrono::duration<r32, std::milli>;

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

#ifdef DEBUG_FEATURES_ENABLE

bool WinApp::IsStopUpdate() {
	auto& instance = GetInstance();
	return instance.isStopUpdate && !instance.isPassedPause;
}

#endif // DEBUG_FEATURES_ENABLE
