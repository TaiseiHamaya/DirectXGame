#include "Engine/WinApp.h"

#include <cstdint>

#include "Engine/Render/RenderPathManager/RenderPathManager.h"
#include "TestCode/SceneDemo.h"
#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Utility/Utility.h"

// クライアント領域サイズ
const std::int32_t kClientWidth = 1280;
const std::int32_t kClientHight = 720;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp::Initialize("DirectXGame", kClientWidth, kClientHight);

	SceneManager::Initialize(CreateUnique<SceneDemo>());

	while (!WinApp::IsEndApp()) {
		WinApp::BeginFrame();

		SceneManager::Begin();

		if (SceneManager::IsEndProgram()) {
			break;
		}

		SceneManager::Update();
		SceneManager::Draw();

		SceneManager::DebugGui();

		RenderPathManager::BeginFrame();
		RenderPathManager::Next();

		WinApp::EndFrame();
	}

	SceneManager::Finalize();

	WinApp::Finalize();
}