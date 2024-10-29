#include "GameScene.h"

#include <Engine/Module/Render/RenderPathManager/RenderPathManager.h>
#include <Engine/Utility/Tools/SmartPointer.h>
#include <Engine/Runtime/WorldClock/WorldClock.h>

void GameScene::load() {
	Rail::LoadMesh();
}

void GameScene::initialize() {
	camera3D = std::make_unique<RailCamera>();
	camera3D->initialize();
	camera3D->set_transform({
		CVector3::BASIS,
		Quaternion::EulerDegree(45,0,0),
		{0,10,-10}
		});

	rail = eps::CreateUnique<Rail>();
	rail->initialize();

	camera3D->set_rail(rail.get());
}

void GameScene::finalize() {
}

void GameScene::poped() {
}

void GameScene::begin() {
}

void GameScene::update() {
	camera3D->update();
}

void GameScene::begin_rendering() {
	camera3D->update_matrix();
	rail->begin_rendering();
}

void GameScene::late_update() {
}

void GameScene::draw() const {
	RenderPathManager::BeginFrame();
	camera3D->set_command(1);
	rail->draw();
#ifdef _DEBUG
	rail->debug_draw();
	camera3D->debug_draw();
#endif // _DEBUG
	RenderPathManager::Next();
}

#ifdef _DEBUG

#include <imgui.h>
void GameScene::debug_update() {

	camera3D->debug_gui();

	ImGui::Begin("WorldClock");
	WorldClock::DebugGui();
	ImGui::End();
}
#endif // _DEBUG
