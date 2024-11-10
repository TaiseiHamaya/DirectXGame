#include "GameScene.h"

#include "Engine/Module/Render/RenderPath/RenderPath.h"
#include "Engine/Rendering/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include <Engine/Module/World/Camera/Camera2D.h>
#include <Engine/Resources/PolygonMesh/PolygonMeshManager.h>
#include <Engine/Resources/Texture/TextureManager.h>
#include <Engine/Runtime/WorldClock/WorldClock.h>
#include <Engine/Utility/Tools/SmartPointer.h>

void GameScene::load() {
	Rail::LoadMesh();
	PolygonMeshManager::RegisterLoadQue("./Resources/Model/", "beam.obj");
	TextureManager::RegisterLoadQue("./Resources/Sprite", "reticle.png");
}

void GameScene::initialize() {
	Camera2D::Initialize();
	camera3D = std::make_unique<RailCamera>();
	camera3D->initialize();
	camera3D->set_transform({
		CVector3::BASIS,
		Quaternion::EulerDegree(45,0,0),
		{0,10,-10}
		});

	rail = eps::CreateUnique<Rail>();
	rail->initialize();

	beam = eps::CreateUnique<Beam>();
	beam->initialize();
	beam->set_camera(camera3D.get());

	directionalLight = eps::CreateUnique<DirectionalLightInstance>();
	directionalLight->initialize();

	camera3D->set_rail(rail.get());

	auto object3dNode = std::make_shared<Object3DNode>();
	object3dNode->initialize();
	object3dNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());
	object3dNode->set_config(eps::to_bitflag(RenderNodeConfig::ContinueDrawBefore));

	auto spriteNode = std::make_shared<SpriteNode>();
	spriteNode->initialize();
	spriteNode->set_config(eps::to_bitflag(RenderNodeConfig::ContinueDrawAfter) | RenderNodeConfig::ContinueDrawBefore);
	spriteNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());
	
	renderPath = eps::CreateUnique<RenderPath>();
	renderPath->initialize({ object3dNode,spriteNode });
}

void GameScene::finalize() {
}

void GameScene::poped() {
}

void GameScene::begin() {
	beam->begin();
}

void GameScene::update() {
	camera3D->update();
	beam->update();
	directionalLight->update();
}

void GameScene::begin_rendering() {
	camera3D->update_matrix();
	rail->begin_rendering();
	beam->begin_rendering();
	directionalLight->begin_rendering();
}

void GameScene::late_update() {
}

void GameScene::draw() const {
	renderPath->begin();
	camera3D->set_command(1);
	directionalLight->register_world(3);
	rail->draw();
	beam->draw();
#ifdef _DEBUG
	rail->debug_draw();
	camera3D->debug_draw();
#endif // _DEBUG

	renderPath->next();
	beam->draw_reticle();
	renderPath->next();
}

#ifdef _DEBUG

#include <imgui.h>
void GameScene::debug_update() {

	camera3D->debug_gui();

	beam->debug_gui();

	ImGui::Begin("WorldClock");
	WorldClock::DebugGui();
	ImGui::End();
}
#endif // _DEBUG
