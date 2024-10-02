#include "SceneDemo.h"

#include "Engine/Module/Camera/Camera3D.h"
#include "Engine/Module/GameObject/GameObject.h"
#include "Engine/Module/Hierarchy/Hierarchy.h"
#include "Engine/Module/PolygonMesh/PolygonMeshManager.h"
#include "Engine/Application/Scene/SceneManager.h"
#include "Engine/Render/RenderPathManager/RenderPathManager.h"
#include "Engine/Utility/Utility.h"
#include "Engine/Module/Collision/Collider/SphereCollider.h"
#include "Engine/Module/Collision/CollisionManager.h"

#include "Engine/Module/Color/Color.h"

#include "Engine/Application/Audio/AudioManager.h"
#include "Engine/Module/TextureManager/TextureManager.h"
#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Engine/Render/RenderTargetGroup/SwapChainRenderTargetGroup.h"
#include "Engine/Render/RenderPath/RenderPath.h"
#include "Engine/DirectX/DirectXCore.h"


SceneDemo::SceneDemo() = default;

SceneDemo::~SceneDemo() = default;

void SceneDemo::load() {
	PolygonMeshManager::RegisterLoadQue("./EngineResources/Models", "Sphere.obj");
	AudioManager::RegisterLoadQue("./EngineResources", "Alarm01.wav");
	// 存在しないファイルをロードしようとするとエラー出力が出る
	AudioManager::RegisterLoadQue("./Engine/Resources", "SE_meteoEachOther.wav");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources", "SE_meteoEachOther.wav");
	TextureManager::RegisterLoadQue("./Engine/Resources", "SE_meteoEachOther.wav");
}

void SceneDemo::initialize() {
	camera3D = std::make_unique<Camera3D>();
	camera3D->initialize();
	camera3D->set_transform({
		CVector3::BASIS,
		Quaternion::EulerDegree(45,0,0),
		{0,10,-10}
		});
	parent = std::make_unique<GameObject>();
	parent->reset_object("Sphere.obj");
	child = std::make_unique<GameObject>();
	child->reset_object("Sphere.obj");
	child->set_parent(parent->get_hierarchy());

	parentCollider = std::make_unique<SphereCollider>();
	parentCollider->initialize();
	parentCollider->get_hierarchy().set_parent(parent->get_hierarchy());

	childCollider = std::make_unique<SphereCollider>();
	childCollider->initialize();
	childCollider->get_hierarchy().set_parent(child->get_hierarchy());

	singleCollider = std::make_unique<SphereCollider>();
	singleCollider->initialize();

	single2Collider = std::make_unique<SphereCollider>();
	single2Collider->initialize();
	single2Collider->get_transform().set_translate_x(-3.0f);

	single3Collider = std::make_unique<SphereCollider>();
	single3Collider->initialize();
	single3Collider->get_transform().set_translate_x(3.0f);

	collisionManager = std::make_unique<CollisionManager>();
	collisionManager->register_collider("Parent", parentCollider);
	collisionManager->register_collider("Single", singleCollider);
	collisionManager->register_collider("Single", single2Collider);
	collisionManager->register_collider("Single", single3Collider);
	collisionManager->register_collider("Child", childCollider);

	audioPlayer = std::make_unique<AudioPlayer>();
	audioPlayer->initialize("Alarm01.wav");

	object3dNode = std::make_unique<Object3DNode>();
	object3dNode->initialize();
	//object3dNode->set_render_target();
	object3dNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());

	//outlineNode = std::make_unique<OutlineNode>();
	//outlineNode->initialize();
	////outlineNode->set_render_target();
	//outlineNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());
	//outlineNode->set_texture_resource(object3dNode->result_stv_handle());
	//outlineNode->set_depth_resource(DirectXSwapChain::GetDepthStencil()->texture_gpu_handle());

	using RTGConfig = BaseRenderTargetGroup::RTGConfing;

	spriteNode = std::make_unique<SpriteNode>();
	spriteNode->initialize();
	spriteNode->set_rt_config(eps::to_bitflag(RTGConfig::ContinueDrawAfter) | RTGConfig::ContinueDrawBefore);
	spriteNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());

	RenderPath path{};
	path.initialize({ object3dNode,spriteNode });

	RenderPathManager::RegisterPath("SceneDemo" + std::to_string(reinterpret_cast<std::uint64_t>(this)), std::move(path));
	RenderPathManager::SetPath("SceneDemo" + std::to_string(reinterpret_cast<std::uint64_t>(this)));

	//DirectXSwapChain::GetRenderTarget()->set_depth_stencil(nullptr);
	//DirectXSwapChain::SetClearColor(Color{ 0.0f,0.0f,0.0f,0.0f });
}

void SceneDemo::poped() {
	RenderPathManager::SetPath("SceneDemo" + std::to_string(reinterpret_cast<std::uint64_t>(this)));
}

void SceneDemo::finalize() {
	audioPlayer->finalize();
	RenderPathManager::UnregisterPath("SceneDemo" + std::to_string(reinterpret_cast<std::uint64_t>(this)));
	object3dNode->finalize();
	//outlineNode->finalize();
}

void SceneDemo::begin() {
}

void SceneDemo::update() {
	camera3D->update();
}

void SceneDemo::begin_rendering() {
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
	camera3D->debug_draw();
	DirectXCore::ShowGrid(*camera3D);
#endif // _DEBUG
	RenderPathManager::Next();
	RenderPathManager::Next();
	//outlineNode->draw();
	//RenderPathManager::Next();
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

#include <imgui.h>
void SceneDemo::debug_update() {
	ImGui::Begin("DemoScene");
	if (ImGui::Button("StackScene")) {
		SceneManager::SetSceneChange(std::make_unique<SceneDemo>(), 1, true);
	}
	if (ImGui::Button("ChangeScene")) {
		SceneManager::SetSceneChange(std::make_unique<SceneDemo>(), 1, false);
	}
	if (ImGui::Button("PopScene")) {
		SceneManager::PopScene(1);
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
	audioPlayer->debug_gui();
	if (ImGui::Button("Play")) {
		audioPlayer->play();
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop")) {
		audioPlayer->stop();
	}
	ImGui::SameLine();
	if (ImGui::Button("Pause")) {
		audioPlayer->pause();
	}
	ImGui::End();

	AudioManager::DebugGui();

	ImGui::Begin("GameTimer");
	GameTimer::DebugGui();
	ImGui::End();

	ImGui::Begin("CollisionManager");
	collisionManager->debug_gui();
	ImGui::End();

	DirectXCore::ShowDebugTools();
}
#endif // _DEBUG
