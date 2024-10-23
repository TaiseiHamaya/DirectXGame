#pragma once

#include "Engine/Application/Scene/BaseScene.h"

#include <memory>

#include "Engine/Application/Audio/AudioPlayer.h"

class GameObject;
class SpriteObject;
class Camera3D;
class BaseCollider;
class SphereCollider;
class CollisionManager;
class Color;

#include "Engine/Render/RenderNode/Object3DNode/Object3DNode.h"
#include "Engine/Render/RenderNode/Sprite/SpriteNode.h"
#include "Engine/Render/RenderNode/Particle/ParticleMeshNode/ParticleMeshNode.h"

#include "Engine/Render/RenderNode/Outline/OutlineNode.h"
#include "Engine/Module/ParticleSystem/ParticleSystemModel.h"

class SceneDemo : public BaseScene {
public:
	SceneDemo();
	~SceneDemo();

public:
	void load() override;
	void initialize() override;
	void poped() override;
	void finalize() override;

	void begin() override;
	void update() override;
	void begin_rendering() override;
	void late_update() override;

	void draw() const override;

	void on_collision(const BaseCollider* const, Color* object);
	void on_collision_enter(const BaseCollider* const, Color* object);
	void on_collision_exit(const BaseCollider* const, Color* object);

#ifdef _DEBUG
	void debug_update() override;
#endif // _DEBUG

private:
	std::unique_ptr<GameObject> parent;
	std::unique_ptr<GameObject> child;
	std::unique_ptr<SpriteObject> sprite;

	std::shared_ptr<SphereCollider> parentCollider;
	std::shared_ptr<SphereCollider> childCollider;
	std::shared_ptr<SphereCollider> singleCollider;
	std::shared_ptr<SphereCollider> single2Collider;
	std::shared_ptr<SphereCollider> single3Collider;

	std::unique_ptr<ParticleSystemModel> particleSystem;

	std::unique_ptr<CollisionManager> collisionManager;
	std::unique_ptr<Camera3D> camera3D;

	std::shared_ptr<AudioPlayer> audioPlayer;

	std::shared_ptr<Object3DNode> object3dNode;
	std::shared_ptr<SpriteNode> spriteNode;
	std::shared_ptr<OutlineNode> outlineNode;
	std::shared_ptr<ParticleMeshNode> particleMeshNode;
};
