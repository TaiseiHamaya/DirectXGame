#include "SceneDemo.h"

#include "Engine/Module/Camera/Camera3D.h"
#include "Engine/Module/GameObject/GameObject.h"
#include "Engine/Module/Hierarchy/Hierarchy.h"
#include "Engine/Module/PolygonMesh/PolygonMeshManager.h"
#include "Engine/Application/Scene/SceneManager.h"
#include "Engine/Render/RenderPathManager/RenderPathManager.h"
#include "Engine/Module/Collision/Collider/SphereCollider.h"
#include "Engine/Module/Collision/CollisionManager.h"

#include "Engine/Module/GameObject/SpriteObject.h"
#include "Engine/Module/Camera/Camera2D.h"

#include "Engine/Module/Color/Color.h"

#include "Engine/Application/Audio/AudioManager.h"
#include "Engine/Module/TextureManager/TextureManager.h"
#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Engine/Render/RenderPath/RenderPath.h"
#include "Engine/DirectX/DirectXCore.h"

#include "Engine/Module/Behavior/Behavior.h"
#include "Engine/Utility/SmartPointer.h"
#include "TestCode/EmitterSample.h"
#include "TestCode/ParticleSample.h"
#include "Engine/Render/RenderTargetGroup/SingleRenderTarget.h"

SceneDemo::SceneDemo() = default;

SceneDemo::~SceneDemo() = default;

void SceneDemo::load() {
	PolygonMeshManager::RegisterLoadQue("./EngineResources/Models", "Sphere.obj");
	AudioManager::RegisterLoadQue("./EngineResources", "Alarm01.wav");
	AudioManager::RegisterLoadQue("./EngineResources/Texture", "CircularGaugeTexter.png");
	// 存在しないファイルをロードしようとするとエラー出力が出る
	AudioManager::RegisterLoadQue("./Engine/Resources", "SE_meteoEachOther.wav");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources", "SE_meteoEachOther.wav");
	TextureManager::RegisterLoadQue("./Engine/Resources", "SE_meteoEachOther.wav");
}

void SceneDemo::initialize() {
	Camera2D::Initialize();
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
	child->set_parent(*parent);

	parentCollider = std::make_unique<SphereCollider>();
	parentCollider->initialize();
	parentCollider->set_parent(*parent);

	childCollider = std::make_unique<SphereCollider>();
	childCollider->initialize();
	childCollider->set_parent(*child);

	singleCollider = std::make_unique<SphereCollider>();
	singleCollider->initialize();

	single2Collider = std::make_unique<SphereCollider>();
	single2Collider->initialize();
	single2Collider->get_transform().set_translate_x(-3.0f);

	single3Collider = std::make_unique<SphereCollider>();
	single3Collider->initialize();
	single3Collider->get_transform().set_translate_x(3.0f);

	particleSystem = eps::CreateUnique<ParticleSystemModel>();
	particleSystem->initialize(128);
	particleSystem->set_emitter(eps::CreateUnique<EmitterSample>());
	particleSystem->set_mesh("Sphere.obj");
	particleSystem->set_particle_movements(
		eps::CreateUnique<ParticleSample>()
	);

	sprite = std::make_unique<SpriteObject>("uvChecker.png");

	collisionManager = std::make_unique<CollisionManager>();
	collisionManager->register_collider("Parent", parentCollider);
	collisionManager->register_collider("Single", singleCollider);
	collisionManager->register_collider("Single", single2Collider);
	collisionManager->register_collider("Single", single3Collider);
	collisionManager->register_collider("Child", childCollider);

	audioPlayer = std::make_unique<AudioPlayer>();
	audioPlayer->initialize("Alarm01.wav");

	std::shared_ptr<SingleRenderTarget> renderTarget = eps::CreateShared<SingleRenderTarget>();
	renderTarget->initialize();

	object3dNode = std::make_unique<Object3DNode>();
	object3dNode->initialize();
	object3dNode->set_render_target(renderTarget);
	object3dNode->set_config(eps::to_bitflag(RenderNodeConfig::ContinueDrawBefore) | RenderNodeConfig::ContinueUseDpehtBefore);

	//object3dNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());

	particleMeshNode = std::make_unique<ParticleMeshNode>();
	particleMeshNode->initialize();
	particleMeshNode->set_render_target(renderTarget);
	particleMeshNode->set_config(eps::to_bitflag(RenderNodeConfig::ContinueDrawAfter) | RenderNodeConfig::ContinueUseDpehtAfter);

	outlineNode = std::make_unique<OutlineNode>();
	outlineNode->initialize();
	//outlineNode->set_render_target();
	outlineNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());
	outlineNode->set_texture_resource(object3dNode->result_stv_handle());
	outlineNode->set_depth_resource(DirectXSwapChain::GetDepthStencil()->texture_gpu_handle());
	outlineNode->set_config(RenderNodeConfig::ContinueDrawBefore);

	spriteNode = std::make_unique<SpriteNode>();
	spriteNode->initialize();
	spriteNode->set_config(eps::to_bitflag(RenderNodeConfig::ContinueDrawAfter) | RenderNodeConfig::ContinueDrawBefore);
	spriteNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());

	RenderPath path{};
	path.initialize({ object3dNode,particleMeshNode,outlineNode,spriteNode });

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
	particleSystem->finalize();
}

void SceneDemo::begin() {
}

void SceneDemo::update() {
	particleSystem->update();
}

void SceneDemo::begin_rendering() {
	camera3D->update_matrix();
	parent->begin_rendering();
	child->look_at(*camera3D);
	child->begin_rendering();
	sprite->begin_rendering();
	particleSystem->begin_rendering();
}

void SceneDemo::late_update() {
	collisionManager->update();
	collisionManager->collision("Parent", "Single");
	collisionManager->collision("Single", "Child");
	collisionManager->collision("Single", "Single");
}

void SceneDemo::draw() const {
	RenderPathManager::BeginFrame();
	camera3D->set_command(1);
	parent->draw();
	child->draw();
#ifdef _DEBUG
	collisionManager->debug_draw3d();
	camera3D->debug_draw();
	DirectXCore::ShowGrid();
#endif // _DEBUG
	RenderPathManager::Next();
	camera3D->set_command(1);
	particleSystem->draw();
	RenderPathManager::Next();
	outlineNode->draw();
	RenderPathManager::Next();
	//sprite->draw();
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
	singleCollider->debug_gui();
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

	ImGui::Begin("WorldClock");
	WorldClock::DebugGui();
	ImGui::End();

	ImGui::Begin("CollisionManager");
	collisionManager->debug_gui();
	ImGui::End();

	ImGui::Begin("ParticleSystem");
	if (ImGui::Button("Emit")) {
		//particleSystem->emit();
	}
	//particleSystem->debug_gui();
	ImGui::End();

	DirectXCore::ShowDebugTools();
}
#endif // _DEBUG
