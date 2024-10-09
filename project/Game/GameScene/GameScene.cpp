#include "GameScene.h"

#include <Engine/Render/RenderPathManager/RenderPathManager.h>
#include <Engine/Utility/SmartPointer.h>
#include <Engine/Application/WorldClock/WorldClock.h>

void GameScene::load() {
	Rail::LoadMesh();
}

void GameScene::initialize() {
	camera3D = std::make_unique<Camera3D>();
	camera3D->initialize();
	camera3D->set_transform({
		CVector3::BASIS,
		Quaternion::EulerDegree(45,0,0),
		{0,10,-10}
		});

	rail = eps::CreateUnique<Rail>();
	rail->initialize();
}

void GameScene::finalize() {
}

void GameScene::poped() {
}

void GameScene::begin() {
}

void GameScene::update() {
}

void GameScene::begin_rendering() {
	camera3D->update_matrix();
}

void GameScene::late_update() {
}

void GameScene::draw() const {
	RenderPathManager::BeginFrame();
	camera3D->set_command(1);
	rail->debug_draw();
	RenderPathManager::Next();
}

#ifdef _DEBUG

#include <imgui.h>
void GameScene::debug_update() {

	ImGui::Begin("Camera3D");
	camera3D->debug_gui();
	ImGui::End();

	ImGui::Begin("WorldClock");
	WorldClock::DebugGui();
	ImGui::End();
}
#endif // _DEBUG
