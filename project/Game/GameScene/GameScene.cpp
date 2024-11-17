#include "GameScene.h"

#include "Engine/Module/Render/RenderPath/RenderPath.h"
#include "Engine/Rendering/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include <Engine/Module/World/Camera/Camera2D.h>
#include <Engine/Resources/PolygonMesh/PolygonMeshManager.h>
#include <Engine/Resources/Texture/TextureManager.h>
#include <Engine/Runtime/WorldClock/WorldClock.h>
#include <Engine/Utility/Tools/SmartPointer.h>

#include <Engine/Module/Render/RenderNode/Object3DNode/Object3DNode.h>
#include <Engine/Module/Render/RenderNode/Sprite/SpriteNode.h>

#include "Collision/GameCollisionCallback.h"
#include "RenderNode/BeamNode.h"

void GameScene::load() {
	Rail::LoadMesh();
	PolygonMeshManager::RegisterLoadQue("./Resources/Model/", "beam.obj");
	PolygonMeshManager::RegisterLoadQue("./Resources/Model/", "enemy.obj");
	TextureManager::RegisterLoadQue("./Resources/Sprite", "reticle.png");
	for (int i = 0; i < 10; ++i) {
		TextureManager::RegisterLoadQue("./Resources/Sprite", std::format("score_sprite_{}.png", i));
	}
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

	enemyManager = eps::CreateUnique<EnemyManager>();

	scoreManager = eps::CreateUnique<ScoreManager>();
	scoreManager->initialize(camera3D.get());

	collisionManager = eps::CreateUnique<CollisionManager>();
	collisionManager->set_callback_manager(
		eps::CreateUnique<GameCollisionCallback>(enemyManager.get())
	);

	enemyManager->initialize(collisionManager.get(), scoreManager.get());

	timeline = eps::CreateUnique<Timeline>();
	timeline->initialize(enemyManager.get(), camera3D.get());

	camera3D->set_rail(rail.get());

	collisionManager->register_collider("Beam", beam->get_collider());

	// レンダリング初期化
	auto object3dNode = std::make_shared<Object3DNode>();
	object3dNode->initialize();
	object3dNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());
	object3dNode->set_config(eps::to_bitflag(RenderNodeConfig::ContinueDrawBefore));

	auto beamNode = std::make_shared<BeamNode>();
	beamNode->initialize();
	beamNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());
	beamNode->set_config(eps::to_bitflag(RenderNodeConfig::ContinueDrawAfter) | RenderNodeConfig::ContinueDrawBefore);

	auto spriteNode = std::make_shared<SpriteNode>();
	spriteNode->initialize();
	spriteNode->set_config(eps::to_bitflag(RenderNodeConfig::ContinueDrawAfter) | RenderNodeConfig::ContinueDrawBefore);
	spriteNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());
	
	renderPath = eps::CreateUnique<RenderPath>();
	renderPath->initialize({ object3dNode,beamNode,spriteNode });
}

void GameScene::finalize() {
}

void GameScene::popped() {
}

void GameScene::begin() {
	beam->begin();
	enemyManager->begin();
	collisionManager->begin();
	scoreManager->begin();
}

void GameScene::update() {
	camera3D->update();
	beam->update();
	directionalLight->update();
	timeline->update();
	enemyManager->update();
	scoreManager->update();
}

void GameScene::begin_rendering() {
	camera3D->update_matrix();
	rail->begin_rendering();
	beam->begin_rendering();
	directionalLight->begin_rendering();
	enemyManager->begin_rendering();
	scoreManager->begin_rendering();
}

void GameScene::late_update() {
	collisionManager->update();
	collisionManager->collision("Enemy", "Beam");

	enemyManager->late_update();
	scoreManager->late_update();
}

void GameScene::draw() const {
	renderPath->begin(); // 3D Mesh
	camera3D->register_world(1);
	directionalLight->register_world(3);

	rail->draw();
	enemyManager->draw();
#ifdef _DEBUG
	rail->debug_draw();
	camera3D->debug_draw();
	collisionManager->debug_draw3d();
#endif // _DEBUG

	renderPath->next(); // Depthなし描画
	beam->draw();
	scoreManager->draw();

	renderPath->next(); // Sprite
	beam->draw_reticle();
	scoreManager->draw_sprite();

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

	ImGui::Begin("CollisionManager");
	collisionManager->debug_gui();
	ImGui::End();

	rail->editor_gui();
	
	enemyManager->debug_gui();

	timeline->debug_gui();
}
#endif // _DEBUG
