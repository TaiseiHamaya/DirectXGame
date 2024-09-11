#include "SceneDemo.h"

#include "Engine/Game/Camera/Camera3D.h"
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Game/Hierarchy/Hierarchy.h"
#include "Engine/Game/Managers/PolygonMeshManager/PolygonMeshManager.h"
#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Render/RenderPathManager/RenderPathManager.h"
#include "Engine/Utility/Utility.h"
#include "Engine/Game/Collision/Collider/SphereCollider.h"
#include "Engine/Game/Collision/CollisionManager/CollisionManager.h"

#include "Engine/Game/Color/Color.h"

#include "Engine/Game/Managers/AudioManager/AudioManager.h"
#include "Engine/Game/Managers/TextureManager/TextureManager.h"
#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Engine/Render/RenderTargetGroup/SwapChainRenderTargetGroup.h"
#include "Engine/Render/RenderPath/RenderPath.h"

SceneDemo::SceneDemo() = default;

SceneDemo::~SceneDemo() = default;

void SceneDemo::load() {
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources/", "Sphere.obj");
	// 存在しないファイルをロードしようとするとエラー出力が出る
	AudioManager::RegisterLoadQue("./Engine/Resources/", "SE_meteoEachOther.wav");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources/", "SE_meteoEachOther.wav");
	TextureManager::RegisterLoadQue("./Engine/Resources/", "SE_meteoEachOther.wav");
}

void SceneDemo::initialize() {
	camera3D = CreateUnique<Camera3D>();
	camera3D->initialize();
	camera3D->set_transform({
		CVector3::BASIS,
		Quaternion::EulerDegree(45,0,0),
		{0,10,-10}
		});
	parent = CreateUnique<GameObject>();
	parent->reset_object("Sphere.obj");
	child = CreateUnique<GameObject>();
	child->reset_object("Sphere.obj");
	child->set_parent(parent->get_hierarchy());

	parentCollider = CreateShared<SphereCollider>();
	parentCollider->initialize();
	parentCollider->set_on_collision(
		std::bind(&SceneDemo::on_collision, this, std::placeholders::_1, &parent->get_materials()[0].color)
	);
	parentCollider->set_on_collision_enter(
		std::bind(&SceneDemo::on_collision_enter, this, std::placeholders::_1, &parent->get_materials()[0].color)
	);
	parentCollider->set_on_collision_exit(
		std::bind(&SceneDemo::on_collision_exit, this, std::placeholders::_1, &parent->get_materials()[0].color)
	);
	parentCollider->get_hierarchy().set_parent(parent->get_hierarchy());

	childCollider = CreateShared<SphereCollider>();
	childCollider->initialize();
	childCollider->set_on_collision(
		std::bind(&SceneDemo::on_collision, this, std::placeholders::_1, &child->get_materials()[0].color)
	);
	childCollider->set_on_collision_exit(
		std::bind(&SceneDemo::on_collision_exit, this, std::placeholders::_1, &child->get_materials()[0].color)
	);
	childCollider->get_hierarchy().set_parent(child->get_hierarchy());

	singleCollider = CreateShared<SphereCollider>();
	singleCollider->initialize();
#ifdef _DEBUG
	singleCollider->set_on_collision_enter(
		std::bind(&SceneDemo::on_collision_enter, this, std::placeholders::_1, &singleCollider->get_collider_drawer().get_materials()[0].color)
	);
	singleCollider->set_on_collision_exit(
		std::bind(&SceneDemo::on_collision_exit, this, std::placeholders::_1, &singleCollider->get_collider_drawer().get_materials()[0].color)
	);
#endif // _DEBUG

	single2Collider = CreateShared<SphereCollider>();
	single2Collider->initialize();
	single2Collider->get_transform().set_translate_x(-3.0f);
#ifdef _DEBUG
	single2Collider->set_on_collision(
		std::bind(&SceneDemo::on_collision, this, std::placeholders::_1, &single2Collider->get_collider_drawer().get_materials()[0].color)
	);
#endif // _DEBUG

	single3Collider = CreateShared<SphereCollider>();
	single3Collider->initialize();
	single3Collider->get_transform().set_translate_x(3.0f);
#ifdef _DEBUG
	single3Collider->set_on_collision_enter(
		std::bind(&SceneDemo::on_collision_enter, this, std::placeholders::_1, &single3Collider->get_collider_drawer().get_materials()[0].color)
	);
#endif // _DEBUG

	collisionManager = CreateUnique<CollisionManager>();
	collisionManager->register_collider("Parent", parentCollider);
	collisionManager->register_collider("Single", singleCollider);
	collisionManager->register_collider("Single", single2Collider);
	collisionManager->register_collider("Single", single3Collider);
	collisionManager->register_collider("Child", childCollider);

	audioPlayer = std::make_unique<AudioPlayer>();
	audioPlayer->initialize("");
	audioPlayer->initialize("SE_meteoEachOther.wav");

	object3dNode = std::make_unique<Object3DNode>();
	object3dNode->initialize();
	object3dNode->set_render_target();
	//object3dNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());
	object3dNode->set_depth_stencil();
	DirectXSwapChain::GetRenderTarget()->set_depth_stencil(nullptr);

	outlineNode = std::make_unique<OutlineNode>();
	outlineNode->initialize();
	//outlineNode->set_render_target();
	outlineNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());
	outlineNode->set_texture_resource(object3dNode->result_stv_handle());
	outlineNode->set_depth_resource(DirectXSwapChain::GetDepthStencil()->texture_gpu_handle());

	//spriteNode = std::make_unique<SpriteNode>();
	//spriteNode->initialize();
	//spriteNode->set_background_texture(outlineNode->result_stv_handle());
	//spriteNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());

	RenderPath path{};
	path.initialize({ object3dNode, outlineNode });

	RenderPathManager::RegisterPath("SceneDemo", std::move(path));
	RenderPathManager::SetPath("SceneDemo");

	//DirectXSwapChain::SetClearColor(Color{ 0.0f,0.0f,0.0f,0.0f });
}

void SceneDemo::finalize() {
	audioPlayer->finalize();
	RenderPathManager::UnregisterPath("SceneDemo");
}

void SceneDemo::begin() {
}

void SceneDemo::update() {
	camera3D->update();
}

void SceneDemo::begin_rendering() {
	camera3D->begin_rendering(*camera3D);
	camera3D->update_matrix();
	parent->begin_rendering(*camera3D);
	child->begin_rendering(*camera3D);
}

void SceneDemo::late_update() {
	collisionManager->update();
	collisionManager->collision("Parent", "Single");
	collisionManager->collision("Single", "Child");
	collisionManager->collision("Single", "Single");
}

void SceneDemo::draw() const {
	RenderPathManager::BeginFrame();
	parent->draw();
	child->draw();
#ifdef _DEBUG
	collisionManager->debug_draw3d(*camera3D);
#endif // _DEBUG
	RenderPathManager::Next();
	outlineNode->draw();
	RenderPathManager::Next();
}

void SceneDemo::on_collision([[maybe_unused]] const BaseCollider* const other, Color* object) {
	*object = { 1.0f,0,0,1.0f };
}

void SceneDemo::on_collision_enter(const BaseCollider* const other, Color* object) {
	*object = { 0,1.0f,0,1.0f };
}

void SceneDemo::on_collision_exit(const BaseCollider* const, Color* object) {
	*object = { 1.0f,1.0f,1.0f,1.0f };
}

#ifdef _DEBUG

#include <externals/imgui/imgui.h>
void SceneDemo::debug_update() {
	ImGui::Begin("DemoScene");
	if (ImGui::Button("StackScene")) {
		SceneManager::SetSceneChange(CreateUnique<SceneDemo>(), true);
	}
	if (ImGui::Button("ChangeScene")) {
		SceneManager::SetSceneChange(CreateUnique<SceneDemo>(), false);
	}
	if (ImGui::Button("PopScene")) {
		SceneManager::PopScene();
	}
	ImGui::End();

	ImGui::Begin("Camera3D");
	camera3D->debug_gui();
	ImGui::End();

	ImGui::Begin("Parent");
	parent->debug_gui();
	ImGui::End();

	ImGui::Begin("Child");
	child->debug_gui();
	ImGui::End();

	ImGui::Begin("Single");
	singleCollider->get_transform().debug_gui();
	ImGui::End();

	ImGui::Begin("Audio");
	if (ImGui::Button("Play")) {
		audioPlayer->restart();
	}
	ImGui::End();
}
#endif // _DEBUG
