#include "SceneDemo.h"

#include <execution>

#include <Library/Math/Color4.h>
#include <Library/Math/Hierarchy.h>
#include <Library/Utility/Template/Behavior.h>
#include <Library/Utility/Tools/RandomEngine.h>
#include <Library/Utility/Tools/SmartPointer.h>

#include "../CallbackManagerDemo.h"

#include <Engine/Application/Output.h>
#include <Engine/Assets/Animation/NodeAnimation/NodeAnimationLibrary.h>
#include <Engine/Assets/Animation/Skeleton/SkeletonLibrary.h>
#include <Engine/Assets/Audio/AudioLibrary.h>
#include <Engine/Assets/Audio/AudioManager.h>
#include <Engine/Assets/PolygonMesh/PolygonMeshLibrary.h>
#include <Engine/Assets/PrimitiveGeometry/PrimitiveGeometryLibrary.h>
#include <Engine/Assets/Texture/TextureLibrary.h>
#include <Engine/Debug/DebugValues/DebugValues.h>
#include <Engine/Debug/ImGui/ImGuiLoadManager/ImGuiLoadManager.h>
#include <Engine/GraphicsAPI/DirectX/DxResource/DepthStencil/DepthStencil.h>
#include <Engine/GraphicsAPI/DirectX/DxSwapChain/DxSwapChain.h>
#include <Engine/Module/Render/RenderNode/Debug/PrimitiveLine/PrimitiveLineNode.h>
#include <Engine/Module/Render/RenderNode/Deferred/Lighting/DirectionalLighingNode.h>
#include <Engine/Module/Render/RenderNode/Deferred/Lighting/PointLightingNode.h>
#include <Engine/Module/Render/RenderNode/Deferred/Mesh/SkinningMeshNodeDeferred.h>
#include <Engine/Module/Render/RenderNode/Deferred/Mesh/StaticMeshNodeDeferred.h>
#include <Engine/Module/Render/RenderPath/RenderPath.h>
#include <Engine/Module/Render/RenderTargetGroup/MultiRenderTarget.h>
#include <Engine/Module/Render/RenderTargetGroup/SingleRenderTarget.h>
#include <Engine/Module/World/Camera/Camera2D.h>
#include <Engine/Module/World/Camera/Camera3D.h>
#include <Engine/Module/World/Collision/Collider/AABBCollider.h>
#include <Engine/Module/World/Collision/Collider/SphereCollider.h>
#include <Engine/Module/World/Collision/CollisionManager.h>
#include <Engine/Module/World/Mesh/SkinningMeshInstance.h>
#include <Engine/Module/World/Mesh/StaticMeshInstance.h>
#include <Engine/Module/World/Sprite/SpriteInstance.h>
#include <Engine/Module/World/WorldManager.h>
#include <Engine/Runtime/Input/InputHandler.h>
#include <Engine/Runtime/Scene/SceneManager.h>

//#define QUATERNION_SERIALIZER
#define TRANSFORM3D_SERIALIZER
#define TRANSFORM2D_SERIALIZER
#define VECTOR3_SERIALIZER
#define VECTOR2_SERIALIZER
#define COLOR3_SERIALIZER
#define COLOR4_SERIALIZER
#include <Engine/Assets/Json/JsonSerializer.h>

#define DEFERRED_RENDERING
//#define FORWARD_RENDERING

SceneDemo::SceneDemo() = default;

SceneDemo::~SceneDemo() = default;

void SceneDemo::load() {
	//PolygonMeshLibrary::RegisterLoadQue("./EngineResources/Models/Misc/bunny.obj");
	PolygonMeshLibrary::RegisterLoadQue("./EngineResources/Models/Misc/teapot.obj");
	PolygonMeshLibrary::RegisterLoadQue("./EngineResources/Models/Primitive/Triangle.obj");
	PolygonMeshLibrary::RegisterLoadQue("./EngineResources/Models/Primitive/Sphere.obj");
	PolygonMeshLibrary::RegisterLoadQue("./EngineResources/Models/Bone/bone.obj");
	//AudioLibrary::RegisterLoadQue("./EngineResources/Alarm01.wav");
	//AudioLibrary::RegisterLoadQue("./EngineResources/Texture/CircularGaugeTexter.png");
	// 存在しないファイルをロードしようとするとエラー出力が出る
	//AudioLibrary::RegisterLoadQue("./Engine/Resources/SE_meteoEachOther.wav");
	//PolygonMeshLibrary::RegisterLoadQue("./Engine/Resources/SE_meteoEachOther.wav");
	//TextureLibrary::RegisterLoadQue("./Engine/Resources/SE_meteoEachOther.wav");

	PolygonMeshLibrary::RegisterLoadQue("./EngineResources/Models/Misc/simpleSkin.gltf");
	NodeAnimationLibrary::RegisterLoadQue("./EngineResources/Models/Misc/simpleSkin.gltf");
	SkeletonLibrary::RegisterLoadQue("./EngineResources/Models/Misc/simpleSkin.gltf");
}

void SceneDemo::initialize() {
	worldManager = eps::CreateUnique<WorldManager>();

	Camera2D::Initialize();
	camera3D = worldManager->create<Camera3D>();
	camera3D->initialize();
	//camera3D->set_transform({
	//	CVector3::BASIS,
	//	Quaternion::EulerDegree(45,0,0),
	//	{0,10,-10}
	//	});
	//camera3D->from_json();

	{
		//animationPlayer.isLoop = true;
		//animationPlayer.timer = 0;
		//animationPlayer.animationName = "animation_AnimatedCube";
		//animationPlayer.nodeName = "AnimatedCube";
		//animationPlayer.nodeAnimation = nodeAnimationResource;
	}

	//testValue = jsonResource.try_emplace<WorldInstance>("name");
	//jsonResource.register_value(__JSON_RESOURCE_REGISTER(testValue));

	parent = worldManager->create<StaticMeshInstance>(nullptr, false, "Player.gltf");
	child = worldManager->create<StaticMeshInstance>(parent, false, "Sphere.obj");

	animatedMeshInstance = worldManager->create<SkinningMeshInstance>(nullptr, false, "Player.gltf", "Idle", true);

	parentCollider = worldManager->create<SphereCollider>(parent, false, 1.0f);

	childCollider = worldManager->create<SphereCollider>(child, false, 1.0f);

	singleCollider = worldManager->create<SphereCollider>(nullptr, false, 1.0f);

	single2Collider = worldManager->create<AABBCollider>(nullptr, false, Vector3{ 3.0f, 2.0f, 1.5f });
	single2Collider->get_transform().set_translate_x(-3.0f);

	single3Collider = worldManager->create<AABBCollider>(nullptr, false, CVector3::BASIS, Vector3{ 0.3f,0.3f,0.3f });
	single3Collider->get_transform().set_translate_x(3.0f);

	//particleEmitter = worldManager->create<ParticleEmitterInstance>(nullptr, false, "test.json", 128);

	sprite = std::make_unique<SpriteInstance>("uvChecker.png");

	uint32_t numPrimitiveR = 20;
	uint32_t numPrimitiveC = 20;
	for (uint32_t i = 0; i < numPrimitiveR; ++i) {
		for (uint32_t j = 0; j < numPrimitiveC; ++j) {
			auto& instance = primitives.emplace_back(worldManager->create<SkinningMeshInstance>(nullptr, false, "simpleSkin.gltf", "Anim_0", true));
			instance->get_transform().set_translate({ (float)i, 1, (float)j });
			instance->get_animation()->set_time_force(RandomEngine::Random01MOD() * 5);
		}
	}

	directionalLight = worldManager->create<DirectionalLightInstance>();
	pointLight = worldManager->create<PointLightInstance>();

	skinningMeshDrawManager = eps::CreateUnique<SkinningMeshDrawManager>();
	skinningMeshDrawManager->initialize(1);
	skinningMeshDrawManager->make_instancing(0, "simpleSkin.gltf", numPrimitiveR * numPrimitiveC);

	staticMeshDrawManager = eps::CreateUnique<StaticMeshDrawManager>();
	staticMeshDrawManager->initialize(1);
	staticMeshDrawManager->make_instancing(0, "Sphere.obj", numPrimitiveR * numPrimitiveC + 128);
	staticMeshDrawManager->make_instancing(0, "bunny.obj", numPrimitiveR * numPrimitiveC);
	staticMeshDrawManager->make_instancing(0, "simpleSkin.gltf", numPrimitiveR * numPrimitiveC);
	staticMeshDrawManager->make_instancing(0, "Triangle.obj", numPrimitiveR * numPrimitiveC);
	staticMeshDrawManager->make_instancing(0, "teapot.obj", numPrimitiveR * numPrimitiveC);
#ifdef DEBUG_FEATURES_ENABLE
	staticMeshDrawManager->register_debug_instance(0, camera3D, true);
#endif // _DEBUG
	staticMeshDrawManager->register_instance(parent);
	staticMeshDrawManager->register_instance(child);

	for (auto& primitive : primitives) {
		skinningMeshDrawManager->register_instance(primitive);
	}
	skinningMeshDrawManager->register_instance(animatedMeshInstance);

	pointLightingExecutor = eps::CreateUnique<PointLightingExecutor>(PrimitiveGeometryLibrary::GetPrimitiveGeometry("Ico3"), 1);
	directionalLightingExecutor = eps::CreateUnique<DirectionalLightingExecutor>(1);

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

#ifdef FORWARD_RENDERING
	///
	/// ここはForward
	/// 
	// ---------------------- RT設定 ----------------------
	std::shared_ptr<SingleRenderTarget> renderTarget = eps::CreateShared<SingleRenderTarget>();
	renderTarget->initialize();

	// ---------------------- StaticMesh ----------------------
	std::shared_ptr<StaticMeshNodeForward> object3dNode;
	object3dNode = std::make_unique<StaticMeshNodeForward>();
	object3dNode->initialize();
	object3dNode->set_render_target_SC(DxSwapChain::GetRenderTarget());
	//object3dNode->set_render_target(renderTarget);
	object3dNode->set_config(RenderNodeConfig::ContinueDrawBefore | RenderNodeConfig::ContinueUseDpehtBefore);
	//object3dNode->set_render_target_SC(DxSwapChain::GetRenderTarget());

	/// ---------------------- SkinMesh ----------------------
	std::shared_ptr<SkinningMeshNodeForward> skinningMeshNode;
	skinningMeshNode = std::make_unique<SkinningMeshNodeForward>();
	skinningMeshNode->initialize();
	skinningMeshNode->set_render_target_SC(DxSwapChain::GetRenderTarget());
	//skinningMeshNode->set_render_target(renderTarget);
	skinningMeshNode->set_config(
		RenderNodeConfig::ContinueDrawBefore |
		RenderNodeConfig::ContinueUseDpehtBefore |
		RenderNodeConfig::ContinueDrawAfter |
		RenderNodeConfig::ContinueUseDpehtAfter
	);
	//object3dNode->set_render_target_SC(DxSwapChain::GetRenderTarget());

	// ---------------------- ParticleBillboard ----------------------
	std::shared_ptr<ParticleMeshNode> particleBillboardNode;
	particleBillboardNode = std::make_unique<ParticleMeshNode>();
	particleBillboardNode->initialize();
	particleBillboardNode->set_config(RenderNodeConfig::ContinueDrawAfter | RenderNodeConfig::ContinueUseDpehtAfter);
	//particleBillboardNode->set_render_target(renderTarget);
	particleBillboardNode->set_render_target_SC(DxSwapChain::GetRenderTarget());

	// ---------------------- Sprite ----------------------
	std::shared_ptr<SpriteNode> spriteNode;
	spriteNode = std::make_unique<SpriteNode>();
	spriteNode->initialize();
	spriteNode->set_config(RenderNodeConfig::ContinueDrawAfter | RenderNodeConfig::ContinueDrawBefore);
	spriteNode->set_render_target_SC(DxSwapChain::GetRenderTarget());

#ifdef DEBUG_FEATURES_ENABLE
	// ---------------------- 線 ----------------------
	std::shared_ptr<PrimitiveLineNode> primitiveLineNode;
	primitiveLineNode = std::make_unique<PrimitiveLineNode>();
	primitiveLineNode->initialize();
#endif // _DEBUG

	// ---------------------- 生成 ----------------------
	renderPath = eps::CreateUnique<RenderPath>();
#ifdef DEBUG_FEATURES_ENABLE
	renderPath->initialize({ object3dNode,skinningMeshNode,particleBillboardNode, spriteNode,primitiveLineNode });
#else
	renderPath->initialize({ object3dNode,skinningMeshNode,particleBillboardNode, spriteNode });
#endif // _DEBUG
#endif // FORWARD_RENDERING

#ifdef DEFERRED_RENDERING
	///
	/// Deferredテスト用
	/// 
	auto deferredRenderTarget = DeferredAdaptor::CreateGBufferRenderTarget();

	auto deferredMeshNode = eps::CreateShared<StaticMeshNodeDeferred>();
	deferredMeshNode->initialize();
	deferredMeshNode->set_render_target(deferredRenderTarget);
	deferredMeshNode->set_config(RenderNodeConfig::ContinueDrawBefore | RenderNodeConfig::ContinueUseDpehtBefore);

	auto skinMeshNodeDeferred = eps::CreateShared<SkinningMeshNodeDeferred>();
	skinMeshNodeDeferred->initialize();
	skinMeshNodeDeferred->set_render_target(deferredRenderTarget);
	skinMeshNodeDeferred->set_config(RenderNodeConfig::ContinueDrawAfter | RenderNodeConfig::ContinueUseDpehtAfter);

	auto directionalLightingNode = eps::CreateShared<DirectionalLightingNode>();
	directionalLightingNode->initialize();
	directionalLightingNode->set_render_target_SC(DxSwapChain::GetRenderTarget());
	directionalLightingNode->set_gbuffers(deferredRenderTarget);

	auto pointLightingNode = eps::CreateShared<PointLightingNode>();
	pointLightingNode->initialize();
	pointLightingNode->set_config(RenderNodeConfig::ContinueDrawAfter);
	pointLightingNode->set_render_target_SC(DxSwapChain::GetRenderTarget());
	pointLightingNode->set_gbuffers(deferredRenderTarget);

	std::shared_ptr<ParticleMeshNode> particleMeshNode;
	particleMeshNode = std::make_unique<ParticleMeshNode>();
	particleMeshNode->initialize();
	particleMeshNode->set_config(RenderNodeConfig::ContinueDrawBefore | RenderNodeConfig::ContinueDrawAfter | RenderNodeConfig::NoClearDepth);
	particleMeshNode->set_render_target_SC(DxSwapChain::GetRenderTarget());

#ifdef DEBUG_FEATURES_ENABLE
	std::shared_ptr<PrimitiveLineNode> primitiveLineNode;
	primitiveLineNode = std::make_unique<PrimitiveLineNode>();
	primitiveLineNode->initialize();
#endif // _DEBUG

	renderPath = eps::CreateUnique<RenderPath>();
#ifdef DEBUG_FEATURES_ENABLE
	renderPath->initialize({ deferredMeshNode,skinMeshNodeDeferred,directionalLightingNode,pointLightingNode,particleMeshNode,primitiveLineNode });
#else
	renderPath->initialize({ deferredMeshNode,skinMeshNodeDeferred,directionalLightingNode,pointLightingNode,particleMeshNode });
#endif // _DEBUG
#endif // DEFERRED_RENDERING

	//DxSwapChain::GetRenderTarget()->set_depth_stencil(nullptr);
	//DxSwapChain::SetClearColor(Color4{ 0.0f,0.0f,0.0f,0.0f });

	Particle::lookAtDefault = camera3D.get();
}

void SceneDemo::popped() {
}

void SceneDemo::finalize() {
}

void SceneDemo::begin() {
	collisionManager->begin();
	animatedMeshInstance->begin();
	//particleEmitter->begin();

	std::for_each(
		primitives.begin(), primitives.end(),
		[](std::unique_ptr<SkinningMeshInstance>& instance) { instance->begin(); }
	);
}

void SceneDemo::update() {
	//animationPlayer.update();
	//parent->get_transform().set_scale(
	//	animationPlayer.calculate_scale()
	//);
	//parent->get_transform().set_quaternion(
	//	animationPlayer.calculate_rotate()
	//);
	//parent->get_transform().set_translate(
	//	animationPlayer.calculate_translate()
	//);

	animatedMeshInstance->update_animation();

	//particleEmitter->update();
	directionalLight->update();
	pointLight->update();

	std::for_each(
		std::execution::par, primitives.begin(), primitives.end(),
		[](std::unique_ptr<SkinningMeshInstance>& instance) { instance->update_animation(); }
	);
}

void SceneDemo::begin_rendering() {
	child->look_at(*camera3D);

	worldManager->update_matrix();

	sprite->transfer();
	//particleEmitter->transfer();
	directionalLight->transfer();
	pointLight->transfer();
	camera3D->transfer();

	pointLightingExecutor->begin();
	directionalLightingExecutor->begin();
	pointLightingExecutor->write_to_buffer(pointLight->transform_matrix(), pointLight->light_data());
	directionalLightingExecutor->write_to_buffer(directionalLight->light_data());
	staticMeshDrawManager->transfer();
	skinningMeshDrawManager->transfer();
}

void SceneDemo::late_update() {
	collisionManager->update();
	//collisionManager->collision("Parent", "Single");
	//collisionManager->collision("Single", "Child");
	//collisionManager->collision("Single", "Single");
}

void SceneDemo::draw() const {
#ifdef FORWARD_RENDERING
	///
	/// Forward
	/// 
	renderPath->begin();
	camera3D->register_world_projection(1);
	camera3D->register_world_lighting(4);
	directionalLight->register_world(5);
	parent->draw();
	child->draw();
#ifdef DEBUG_FEATURES_ENABLE
	camera3D->debug_draw_axis();
	animatedMeshInstance->draw_skeleton();
	DebugValues::ShowGrid();
#endif // _DEBUG

	renderPath->next();
	camera3D->register_world_projection(1);
	camera3D->register_world_lighting(5);
	directionalLight->register_world(6);

	animatedMeshInstance->draw();

	renderPath->next();
	camera3D->register_world_projection(1);
	particleEmitter->draw();

	renderPath->next();
	sprite->draw();

	renderPath->next();

#ifdef DEBUG_FEATURES_ENABLE
	camera3D->register_world_projection(1);
	collisionManager->debug_draw3d();
	camera3D->debug_draw_frustum();

	renderPath->next();
#endif // _DEBUG
#endif // FORWARD_RENDERING

#ifdef DEFERRED_RENDERING
	///
	/// Deferred
	/// 
	renderPath->begin();
	camera3D->register_world_projection(2);
	staticMeshDrawManager->draw_layer(0);

	renderPath->next();
	camera3D->register_world_projection(2);
	skinningMeshDrawManager->draw_layer(0);

	renderPath->next();
	camera3D->register_world_lighting(1);
	directionalLightingExecutor->draw_command();

	renderPath->next();
	camera3D->register_world_projection(1);
	camera3D->register_world_lighting(6);
	pointLightingExecutor->draw_command();

	renderPath->next();
	camera3D->register_world_projection(1);
	//particleEmitter->draw();

	renderPath->next();

#ifdef DEBUG_FEATURES_ENABLE
	camera3D->register_world_projection(1);
	collisionManager->debug_draw3d();
	camera3D->debug_draw_frustum();

	renderPath->next();
#endif // _DEBUG
#endif // DEFERRED_RENDERING
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

#ifdef DEBUG_FEATURES_ENABLE

#include <imgui.h>
void SceneDemo::debug_update() {
	ImGui::Begin("DemoScene");
	if (ImGui::Button("StackScene")) {
		SceneManager::SetSceneChange(0, 1, true);
	}
	if (ImGui::Button("ChangeScene")) {
		SceneManager::SetSceneChange(0, 1, false);
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

	ImGui::Begin("Parent");
	parent->debug_gui();
	ImGui::End();

	ImGui::Begin("Child");
	child->debug_gui();
	ImGui::End();

	ImGui::Begin("Sprite");
	sprite->debug_gui();
	ImGui::End();

	AudioManager::DebugGui();

	ImGui::Begin("WorldClock");
	WorldClock::DebugGui();
	ImGui::End();

	ImGui::Begin("CollisionManager");
	collisionManager->debug_gui();
	ImGui::End();

	ImGui::Begin("DirectionalLight");
	directionalLight->debug_gui();
	ImGui::End();

	ImGui::Begin("PointLight");
	pointLight->debug_gui();
	ImGui::End();

	//ImGui::Begin("Particle");
	//particleEmitter->debug_gui();
	//ImGui::End();

	ImGui::Begin("Audio");
	audioPlayer->debug_gui();
	if (ImGui::Button("Play")) {
		audioPlayer->play();
	}
	if (ImGui::Button("Stop")) {
		audioPlayer->stop();
	}
	if (ImGui::Button("Pause")) {
		audioPlayer->pause();
	}
	ImGui::End();

	ImGui::Begin("StaticMeshDrawManager");
	staticMeshDrawManager->debug_gui();
	ImGui::End();

	ImGui::Begin("SkinningMeshDrawManager");
	skinningMeshDrawManager->debug_gui();
	ImGui::End();

	ImGuiLoadManager::ShowGUI();
}
#endif // _DEBUG
