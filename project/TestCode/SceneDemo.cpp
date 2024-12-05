#include "SceneDemo.h"

#include "CallbackManagerDemo.h"
#include "Engine/Module/World/Camera/Camera3D.h"
#include "Engine/Module/World/Collision/Collider/SphereCollider.h"
#include "Engine/Module/World/Collision/CollisionManager.h"
#include "Engine/Module/World/Mesh/MeshInstance.h"
#include "Engine/Resources/PolygonMesh/PolygonMeshManager.h"
#include "Engine/Runtime/Scene/SceneManager.h"
#include "Library/Math/Hierarchy.h"

#include "Engine/Module/World/Camera/Camera2D.h"
#include "Engine/Module/World/Sprite/SpriteInstance.h"

#include "Library/Math/Color4.h"

#include "Engine/Debug/DebugValues/DebugValues.h"
#include "Engine/Module/Render/RenderPath/RenderPath.h"
#include "Engine/Rendering/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Engine/Resources/Audio/AudioManager.h"
#include "Engine/Resources/Texture/TextureManager.h"

#include "Engine/Module/Render/RenderTargetGroup/SingleRenderTarget.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/DepthStencil/DepthStencil.h"
#include "Engine/Utility/Template/Behavior.h"
#include "Engine/Utility/Tools/SmartPointer.h"
#include "TestCode/EmitterSample.h"
#include "TestCode/ParticleFactorySample.h"
#include "TestCode/ParticleSample.h"

//#define QUATERNION_SERIALIZER
#define TRANSFORM3D_SERIALIZER
#define TRANSFORM2D_SERIALIZER
#define VECTOR3_SERIALIZER
#define VECTOR2_SERIALIZER
#define COLOR3_SERIALIZER
#define COLOR4_SERIALIZER
#include <Engine/Resources/Json/JsonSerializer.h>

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
	//camera3D->from_json();


	//testValue = jsonResource.try_emplace<WorldInstance>("name");
	jsonResource.register_value(__JSON_RESOURCE_REGISTER(testValue));

	parent = std::make_unique<MeshInstance>();
	parent->reset_object("Sphere.obj");
	child = std::make_unique<MeshInstance>();
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

	particleSystem = eps::CreateUnique<ParticleSystemBillboard>();
	particleSystem->initialize(128);
	particleSystem->set_texture("uvChecker.png");
	particleSystem->create_rect(CVector2::BASIS);
	particleSystem->set_emitter(eps::CreateUnique<EmitterSample>());
	particleSystem->set_factory(eps::CreateUnique<ParticleFactorySample>(camera3D.get()));

	sprite = std::make_unique<SpriteInstance>("uvChecker.png");

	directionalLight = eps::CreateUnique<DirectionalLightInstance>();
	directionalLight->initialize();

	collisionManager = std::make_unique<CollisionManager>();
	collisionManager->set_callback_manager(
		eps::CreateUnique<CallbackManagerDemo>()
	);
	collisionManager->register_collider("Parent", parentCollider);
	collisionManager->register_collider("Single", singleCollider);
	collisionManager->register_collider("Single", single2Collider);
	collisionManager->register_collider("Single", single3Collider);
	collisionManager->register_collider("Child", childCollider);

	audioPlayer = std::make_unique<AudioPlayer>();
	audioPlayer->initialize("Alarm01.wav");

	std::shared_ptr<SingleRenderTarget> renderTarget = eps::CreateShared<SingleRenderTarget>();
	renderTarget->initialize();

	std::shared_ptr<Object3DNode> object3dNode;
	object3dNode = std::make_unique<Object3DNode>();
	object3dNode->initialize();
	object3dNode->set_render_target(renderTarget);
	object3dNode->set_config(eps::to_bitflag(RenderNodeConfig::ContinueDrawBefore) | RenderNodeConfig::ContinueUseDpehtBefore);
	//object3dNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());

	std::shared_ptr<ParticleBillboardNode> particleBillboardNode;
	particleBillboardNode = std::make_unique<ParticleBillboardNode>();
	particleBillboardNode->initialize();
	particleBillboardNode->set_render_target(renderTarget);
	particleBillboardNode->set_config(eps::to_bitflag(RenderNodeConfig::ContinueDrawAfter) | RenderNodeConfig::ContinueUseDpehtAfter);

	outlineNode = std::make_unique<OutlineNode>();
	outlineNode->initialize();
	//outlineNode->set_render_target();
	outlineNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());
	outlineNode->set_texture_resource(object3dNode->result_stv_handle());
	outlineNode->set_depth_resource(DepthStencilValue::depthStencil->texture_gpu_handle());
	outlineNode->set_config(RenderNodeConfig::ContinueDrawBefore);

	std::shared_ptr<SpriteNode> spriteNode;
	spriteNode = std::make_unique<SpriteNode>();
	spriteNode->initialize();
	spriteNode->set_config(eps::to_bitflag(RenderNodeConfig::ContinueDrawAfter) | RenderNodeConfig::ContinueDrawBefore);
	spriteNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());

	renderPath = eps::CreateUnique<RenderPath>();
	renderPath->initialize({ object3dNode,particleBillboardNode,outlineNode,spriteNode });

	//DirectXSwapChain::GetRenderTarget()->set_depth_stencil(nullptr);
	//DirectXSwapChain::SetClearColor(Color4{ 0.0f,0.0f,0.0f,0.0f });
}

void SceneDemo::popped() {
}

void SceneDemo::finalize() {
}

void SceneDemo::begin() {
	collisionManager->begin();
}

void SceneDemo::update() {
	particleSystem->update();
	directionalLight->update();
}

void SceneDemo::begin_rendering() {
	camera3D->update_matrix();
	parent->begin_rendering();
	child->look_at(*camera3D);
	child->begin_rendering();
	sprite->begin_rendering();
	particleSystem->begin_rendering();
	directionalLight->begin_rendering();
}

void SceneDemo::late_update() {
	collisionManager->update();
	collisionManager->collision("Parent", "Single");
	collisionManager->collision("Single", "Child");
	collisionManager->collision("Single", "Single");
}

void SceneDemo::draw() const {
	renderPath->begin();
	directionalLight->register_world(3);
	camera3D->register_world(1);
	parent->draw();
	child->draw();
#ifdef _DEBUG
	collisionManager->debug_draw3d();
	camera3D->debug_draw();
	DebugValues::ShowGrid();
#endif // _DEBUG

	renderPath->next();
	camera3D->register_world(1);
	particleSystem->draw();

	renderPath->next();
	outlineNode->draw();

	renderPath->next();
	//sprite->draw();

}

void SceneDemo::on_collision([[maybe_unused]] const BaseCollider* const other, Color4* object) {
	*object = { 1.0f,0,0,1.0f };
}

void SceneDemo::on_collision_enter(const BaseCollider* const other, Color4* object) {
	*object = { 0,1.0f,0,1.0f };
}

void SceneDemo::on_collision_exit(const BaseCollider* const, Color4* object) {
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

	ImGui::Begin("Sprite");
	sprite->debug_gui();
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
	particleSystem->debug_gui();
	ImGui::End();

	ImGui::Begin("DirectionalLight");
	directionalLight->debug_gui();
	ImGui::End();

	ImGui::Begin("TestImGui");
	jsonResource.show_imgui();
	if (ImGui::Button("Save")) {
		
		jsonResource.save();
	}
	ImGui::End();
}
#endif // _DEBUG
