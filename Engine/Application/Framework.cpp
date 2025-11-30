#include "Framework.h"

#include "Engine/Application/WinApp.h"

void Framework::run() {
	WinApp::Initialize();

	initialize();

	WinApp::ShowAppWindow();

	while (true) {
		WinApp::BeginFrame();

		WinApp::ProcessMessage();

		if (WinApp::IsEndApp()) {
			break;
		}

		WinApp::Update();

		WinApp::Draw();

		WinApp::EndFrame();
	}

	WinApp::Finalize();
}
