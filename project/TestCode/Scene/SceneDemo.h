#pragma once

#include "Engine/Runtime/Scene/BaseScene.h"

#include <memory>

#include "Engine/Assets/Audio/AudioPlayer.h"

class WorldManager;
class StaticMeshInstance;
class SkinningMeshInstance;
class SpriteInstance;
class Camera3D;
class BaseCollider;
class SphereCollider;
class AABBCollider;
class CollisionManager;
class Color4;
class RenderPath;
#include "Engine/Module/World/Light/DirectionalLight/DirectionalLightInstance.h"
#include "Engine/Module/World/Light/PointLight/PointLightInstance.h"

#include "Engine/Module/Render/RenderNode/Forward/Mesh/StaticMeshNodeForward.h"
#include "Engine/Module/Render/RenderNode/Forward/Mesh/SkinningMeshNodeForward.h"
#include "Engine/Module/Render/RenderNode/2D/Sprite/SpriteNode.h"
#include "Engine/Module/Render/RenderNode/Forward/Particle/ParticleBillboardNode/ParticleBillboardNode.h"
#include "Engine/Module/Render/RenderNode/Forward/Particle/ParticleMeshNode/ParticleMeshNode.h"
#include "Engine/Module/Render/RenderNode/Debug/PrimitiveLine/PrimitiveLineNode.h"

#include "Engine/Module/Render/RenderNode/Posteffect/Outline/OutlineNode.h"
#include "Engine/Module/World/Particle/ParticleEmitterInstance.h"

#include "Engine/Assets/Json/JsonAsset.h"
#include "Engine/Debug/ImGui/ImGuiJsonEditor/ImGuiValueEditor.h"

#include "Engine/Assets/Animation/NodeAnimation/NodeAnimationAsset.h"
#include "Engine/Assets/Animation/NodeAnimation/NodeAnimationPlayer.h"
#include "Engine/Assets/Animation/Skeleton/SkeletonAsset.h"

#include "Engine/Module/DrawExecutor/LightingExecutor/DirectionalLightingExecutor.h"
#include "Engine/Module/DrawExecutor/LightingExecutor/PointLightingExecutor.h"

class SceneDemo : public BaseScene {
public:
	SceneDemo();
	~SceneDemo();

public:
	void load() override;
	void initialize() override;
	void popped() override;
	void finalize() override;

	void begin() override;
	void update() override;
	void begin_rendering() override;
	void late_update() override;

	void draw() const override;

	void on_collision(const BaseCollider* const, Color4* object);
	void on_collision_enter(const BaseCollider* const, Color4* object);
	void on_collision_exit(const BaseCollider* const, Color4* object);

#ifdef _DEBUG
	void debug_update() override;
#endif // _DEBUG

private:
	std::unique_ptr<RenderPath> renderPath;
	std::unique_ptr<WorldManager> worldManager;

	std::unique_ptr<DirectionalLightingExecutor> directionalLightingExecutor;
	std::unique_ptr<PointLightingExecutor> pointLightingExecutor;

	std::unique_ptr<StaticMeshInstance> parent;
	std::unique_ptr<StaticMeshInstance> child;
	std::unique_ptr<SkinningMeshInstance> animatedMeshInstance;
	std::unique_ptr<SpriteInstance> sprite;

	std::unique_ptr<DirectionalLightInstance> directionalLight;
	std::unique_ptr<PointLightInstance> pointLight;

	std::shared_ptr<SphereCollider> parentCollider;
	std::shared_ptr<SphereCollider> childCollider;
	std::shared_ptr<SphereCollider> singleCollider;
	std::shared_ptr<AABBCollider> single2Collider;
	std::shared_ptr<AABBCollider> single3Collider;

	std::shared_ptr<NodeAnimationAsset> nodeAnimationResource;
	NodeAnimationPlayer animationPlayer;
	std::shared_ptr<SkeletonAsset> skeletonResource;

	std::unique_ptr<ParticleEmitterInstance> particleEmitter;

	std::unique_ptr<CollisionManager> collisionManager;
	std::unique_ptr<Camera3D> camera3D;

	std::shared_ptr<AudioPlayer> audioPlayer;
	//std::shared_ptr<OutlineNode> outlineNode;

	JsonAsset jsonResource{ "./Test.json" };
	Transform2D testValue{};
};
