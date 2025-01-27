#include "Framework.h"

#include "Engine/Application/WinApp.h"
#include "Engine/Runtime/Scene/SceneManager.h"

void Framework::run() {
	initialize();

	while (true) {
		begin_frame();

		if (WinApp::IsEndApp()) {
			break;
		}

		update();

		draw();

		end_frame();
	}

	finalize();
}

void Framework::initialize() {
	WinApp::Initialize();
#ifdef _DEBUG
	WorldClock::IsFixDeltaTime(false);
#endif // _DEBUG
	SceneManager::Initialize();
	WinApp::ShowAppWindow();
}

void Framework::finalize() {
	WinApp::Finalize();
}

void Framework::begin_frame() {
	WinApp::BeginFrame();
	SceneManager::Begin();
	WinApp::ProcessMessage();
}

void Framework::update() {
	SceneManager::Update();
}

void Framework::draw() const {
	SceneManager::Draw();
}

void Framework::end_frame() {
#ifdef _DEBUG
	SceneManager::DebugGui();
#endif // _DEBUG
	WinApp::EndFrame();
}
