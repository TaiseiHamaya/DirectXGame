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
}

void SceneDemo::initialize() {
	camera3D = std::make_unique<Camera3D>();
	camera3D->initialize();
	//camera3D->set_transform({
	//	CVector3::BASIS,
	//	Quaternion::EulerDegree(45,0,0),
	//	{0,10,-10}
	//	});
	//camera3D->from_json();

	particleEmitter = eps::CreateUnique<ParticleEmitterInstance>("test.json", 128);
	particleEmitter->initialize();

	directionalLight = eps::CreateUnique<DirectionalLightInstance>();
	directionalLight->initialize();

	std::shared_ptr<Object3DNode> object3dNode;
	object3dNode = std::make_unique<Object3DNode>();
	object3dNode->initialize();
	object3dNode->set_config(eps::to_bitflag(RenderNodeConfig::ContinueDrawBefore) | RenderNodeConfig::ContinueUseDpehtBefore);
	object3dNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());

	std::shared_ptr<ParticleBillboardNode> particleBillboardNode;
	particleBillboardNode = std::make_unique<ParticleBillboardNode>();
	particleBillboardNode->initialize();
	particleBillboardNode->set_config(eps::to_bitflag(RenderNodeConfig::ContinueDrawAfter) | RenderNodeConfig::ContinueUseDpehtAfter);
	particleBillboardNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());

	std::shared_ptr<SpriteNode> spriteNode;
	spriteNode = std::make_unique<SpriteNode>();
	spriteNode->initialize();
	spriteNode->set_config(eps::to_bitflag(RenderNodeConfig::ContinueDrawAfter) | RenderNodeConfig::ContinueDrawBefore);
	spriteNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());

	renderPath = eps::CreateUnique<RenderPath>();
	renderPath->initialize({ object3dNode,particleBillboardNode,spriteNode });

	//DirectXSwapChain::GetRenderTarget()->set_depth_stencil(nullptr);
	//DirectXSwapChain::SetClearColor(Color4{ 0.0f,0.0f,0.0f,0.0f });

	Particle::lookAtDefault = camera3D.get();
}

void SceneDemo::popped() {
}

void SceneDemo::finalize() {
}

void SceneDemo::begin() {
}

void SceneDemo::update() {
	particleEmitter->update();
	directionalLight->update();
}

void SceneDemo::begin_rendering() {
	camera3D->update_matrix();
	particleEmitter->begin_rendering();
	directionalLight->begin_rendering();
}

void SceneDemo::late_update() {
}

void SceneDemo::draw() const {
	renderPath->begin();
	directionalLight->register_world(3);
	camera3D->register_world(1);
#ifdef _DEBUG
	camera3D->debug_draw();
	DebugValues::ShowGrid();
#endif // _DEBUG

	renderPath->next();
	camera3D->register_world(1);
	particleEmitter->draw();

	renderPath->next();
	//sprite->draw();

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

	AudioManager::DebugGui();

	ImGui::Begin("WorldClock");
	WorldClock::DebugGui();
	ImGui::End();

	ImGui::Begin("Particle");
	if (ImGui::Button("Emit")) {
		//particleSystem->emit();
	}
	particleEmitter->debug_gui();
	ImGui::End();

	ImGui::Begin("DirectionalLight");
	directionalLight->debug_gui();
	ImGui::End();
}
#endif // _DEBUG
