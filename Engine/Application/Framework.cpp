#include "Framework.h"

#include "Engine/Application/WinApp.h"
#include "Engine/Runtime/Scene/SceneManager.h"

void Framework::run() {
	WinApp::Initialize();
	initialize();

	WinApp::ShowAppWindow();

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
	SceneManager::Initialize();
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
	WinApp::EndFrame();
}
