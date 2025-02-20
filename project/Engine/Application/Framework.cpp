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
	SceneManager::Initialize();
	WinApp::ShowAppWindow();

#ifdef _DEBUG
	SceneManager::SetProfiler(profiler);
#endif // _DEBUG
}

void Framework::finalize() {
	WinApp::Finalize();
}

void Framework::begin_frame() {
#ifdef _DEBUG
	profiler.clear_timestamps();
	profiler.timestamp("BeginFrame");
#endif // _DEBUG
	WinApp::BeginFrame();
	SceneManager::Begin();
	WinApp::ProcessMessage();
}

void Framework::update() {
	SceneManager::Update();
}

void Framework::draw() const {
#ifdef _DEBUG
	profiler.timestamp("Draw");
#endif // _DEBUG
	SceneManager::Draw();
}

void Framework::end_frame() {
#ifdef _DEBUG
	profiler.timestamp("EndFrame");
	SceneManager::DebugGui();
	profiler.timestamp("End");
	ImGui::Begin("Profiler");
	profiler.debug_gui();
	ImGui::End();
#endif // _DEBUG
	WinApp::EndFrame();
}
