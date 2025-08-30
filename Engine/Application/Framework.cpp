#include "Framework.h"

#include "Engine/Application/WinApp.h"

void Framework::run() {
	WinApp::Initialize();

	WinApp::ShowAppWindow();

	while (true) {
		WinApp::BeginFrame();

		if (WinApp::IsEndApp()) {
			break;
		}

		WinApp::Update();

		WinApp::Draw();

		WinApp::EndFrame();
	}

	WinApp::Finalize();
}
