#include "SceneDemo.h"

#include "CallbackManagerDemo.h"
#include "Engine/Module/World/AnimatedMesh/AnimatedMeshInstance.h"
#include "Engine/Module/World/Camera/Camera3D.h"
#include "Engine/Module/World/Collision/Collider/SphereCollider.h"
#include "Engine/Module/World/Collision/Collider/AABBCollider.h"
#include "Engine/Module/World/Collision/CollisionManager.h"
#include "Engine/Module/World/Mesh/MeshInstance.h"
#include "Engine/Resources/Animation/NodeAnimation/NodeAnimationManager.h"
#include "Engine/Resources/Animation/Skeleton/SkeletonManager.h"
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
#include "Engine/Module/Render/RenderTargetGroup/MultiRenderTarget.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/DepthStencil/DepthStencil.h"
#include "Engine/Utility/Template/Behavior.h"
#include "Engine/Utility/Tools/SmartPointer.h"

#include "Engine/Module/Render/RenderNode/Deferred/Mesh/MeshNodeDeferred.h"
#include "Engine/Module/Render/RenderNode/Deferred/SkinMesh/SkinMeshNodeDeferred.h"
#include "Engine/Module/Render/RenderNode/Deferred/Lighting/DirectionalLighingNode.h"
#include "Engine/Module/Render/RenderNode/Debug/PrimitiveLine/PrimitiveLineNode.h"
#include "Engine/Debug/ImGui/ImGuiLoadManager/ImGuiLoadManager.h"

SceneDemo::SceneDemo() = default;

SceneDemo::~SceneDemo() = default;

void SceneDemo::load() {
	PolygonMeshManager::RegisterLoadQue("./EngineResources/Models/skin-sphere.gltf");
	NodeAnimationManager::RegisterLoadQue("./EngineResources/Models/skin-sphere.gltf");
	SkeletonManager::RegisterLoadQue("./EngineResources/Models/skin-sphere.gltf");
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

	animatedMeshInstance = eps::CreateUnique<AnimatedMeshInstance>("skin-sphere.gltf", "animaiton", true);

	directionalLight = eps::CreateUnique<DirectionalLightInstance>();

	///
	/// Deferred
	/// 
	auto deferredRenderTarget = DeferredAdaptor::CreateGBufferRenderTarget();

	auto deferredMeshNode = eps::CreateShared<MeshNodeDeferred>();
	deferredMeshNode->initialize();
	deferredMeshNode->set_render_target(deferredRenderTarget);
	deferredMeshNode->set_config(RenderNodeConfig::ContinueDrawBefore | RenderNodeConfig::ContinueUseDpehtBefore);

	auto skinMeshNodeDeferred = eps::CreateShared<SkinMeshNodeDeferred>();
	skinMeshNodeDeferred->initialize();
	skinMeshNodeDeferred->set_render_target(deferredRenderTarget);
	skinMeshNodeDeferred->set_config(RenderNodeConfig::ContinueDrawAfter | RenderNodeConfig::ContinueUseDpehtAfter);

	auto directionalLightingNode = eps::CreateShared<DirectionalLightingNode>();
	directionalLightingNode->initialize();
	directionalLightingNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());
	directionalLightingNode->set_gbuffers(deferredRenderTarget);

#ifdef _DEBUG
	std::shared_ptr<PrimitiveLineNode> primitiveLineNode;
	primitiveLineNode = std::make_unique<PrimitiveLineNode>();
	primitiveLineNode->initialize();
#endif // _DEBUG

	renderPath = eps::CreateUnique<RenderPath>();
#ifdef _DEBUG
	renderPath->initialize({ deferredMeshNode,skinMeshNodeDeferred ,directionalLightingNode,primitiveLineNode });
#else
	renderPath->initialize({ deferredMeshNode,skinMeshNodeDeferred,directionalLightingNode });
#endif // _DEBUG
}

void SceneDemo::popped() {
}

void SceneDemo::finalize() {
}

void SceneDemo::begin() {
	animatedMeshInstance->begin();
}

void SceneDemo::update() {
	animatedMeshInstance->update();
	directionalLight->update();
}

void SceneDemo::begin_rendering() {
	camera3D->update_matrix();
	directionalLight->begin_rendering();

	animatedMeshInstance->begin_rendering();
}

void SceneDemo::late_update() {
}

void SceneDemo::draw() const {

	///
	/// Deferred
	/// 
	renderPath->begin();
	camera3D->register_world_projection(1);
#ifdef _DEBUG
	camera3D->debug_draw_axis();
	DebugValues::ShowGrid();
#endif // _DEBUG

	renderPath->next();
	camera3D->register_world_projection(1);
	animatedMeshInstance->draw();

	renderPath->next();
	directionalLight->register_world(0);
	camera3D->register_world_lighting(1);
	directionalLight->draw_deferred();

	renderPath->next();

#ifdef _DEBUG
	camera3D->register_world_projection(1);
	camera3D->debug_draw_frustum();

	renderPath->next();
#endif // _DEBUG

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

	ImGui::Begin("AnimatedMesh");
	animatedMeshInstance->debug_gui();
	ImGui::End();

	AudioManager::DebugGui();

	ImGui::Begin("WorldClock");
	WorldClock::DebugGui();
	ImGui::End();

	ImGui::Begin("DirectionalLight");
	directionalLight->debug_gui();
	ImGui::End();

	ImGuiLoadManager::ShowGUI();
}
#endif // _DEBUG
