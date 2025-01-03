#pragma once

#include "Engine/Runtime/Scene/BaseScene.h"

#include <memory>

#include "Engine/Resources/Audio/AudioPlayer.h"

class MeshInstance;
class AnimatedMeshInstance;
class SpriteInstance;
class Camera3D;
class BaseCollider;
class SphereCollider;
class CollisionManager;
class Color4;
class RenderPath;
#include "Engine/Module/World/Light/DirectionalLight/DirectionalLightInstance.h"

#include "Engine/Module/Render/RenderNode/Object3DNode/Object3DNode.h"
#include "Engine/Module/Render/RenderNode/SkinningMesh/SkinningMeshNode.h"
#include "Engine/Module/Render/RenderNode/Sprite/SpriteNode.h"
#include "Engine/Module/Render/RenderNode/Particle/ParticleBillboardNode/ParticleBillboardNode.h"
#include "Engine/Module/Render/RenderNode/Particle/ParticleMeshNode/ParticleMeshNode.h"

#include "Engine/Module/Render/RenderNode/Outline/OutlineNode.h"
#include "Engine/Module/World/Particle/ParticleEmitterInstance.h"

#include "Engine/Resources/Json/JsonResource.h"
#include "Engine/Debug/ImGui/ImGuiJsonEditor/ImGuiValueEditor.h"

#include "Engine/Resources/Animation/NodeAnimation/NodeAnimationResource.h"
#include "Engine/Resources/Animation/NodeAnimation/NodeAnimationPlayer.h"
#include "Engine/Resources/Animation/Skeleton/SkeletonResource.h"

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

	std::unique_ptr<MeshInstance> parent;
	std::unique_ptr<MeshInstance> child;
	std::unique_ptr<AnimatedMeshInstance> animatedMeshInstance;
	std::unique_ptr<SpriteInstance> sprite;
	std::unique_ptr<DirectionalLightInstance> directionalLight;

	std::shared_ptr<SphereCollider> parentCollider;
	std::shared_ptr<SphereCollider> childCollider;
	std::shared_ptr<SphereCollider> singleCollider;
	std::shared_ptr<SphereCollider> single2Collider;
	std::shared_ptr<SphereCollider> single3Collider;

	std::shared_ptr<NodeAnimationResource> nodeAnimationResource;
	NodeAnimationPlayer animationPlayer;
	std::shared_ptr<SkeletonResource> skeletonResource;

	std::unique_ptr<ParticleEmitterInstance> particleEmitter;

	std::unique_ptr<CollisionManager> collisionManager;
	std::unique_ptr<Camera3D> camera3D;

	std::shared_ptr<AudioPlayer> audioPlayer;
	//std::shared_ptr<OutlineNode> outlineNode;

	JsonResource jsonResource{ "./Test.json" };
	Transform2D testValue{};
};
