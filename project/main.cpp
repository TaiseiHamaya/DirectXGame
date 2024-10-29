#include "Engine/Application/WinApp.h"

#include <cstdint>

#include "Engine/Runtime/Scene/SceneManager.h"
#include "TestCode/SceneDemo.h"

// クライアント領域サイズ
const std::int32_t kClientWidth = 1280;
const std::int32_t kClientHight = 720;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp::Initialize("DirectXGame", kClientWidth, kClientHight);

#ifdef _DEBUG
	WorldClock::IsFixDeltaTime(false);
#endif // _DEBUG

	SceneManager::Initialize(std::make_unique<SceneDemo>());

	WinApp::ShowAppWindow();

	while (true) {
		WinApp::BeginFrame();

		SceneManager::Begin();

		WinApp::ProcessMessage();

		if (WinApp::IsEndApp()) {
			break;
		}

		SceneManager::Update();

		SceneManager::Draw();

#ifdef _DEBUG
		SceneManager::DebugGui();
#endif // _DEBUG

		WinApp::EndFrame();
	}

	WinApp::Finalize();
}
