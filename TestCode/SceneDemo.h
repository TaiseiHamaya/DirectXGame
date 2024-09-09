#pragma once

#include "Engine/Game/Scene/BaseScene.h"

#include <memory>

class GameObject;
class Camera3D;
class BaseCollider;
class SphereCollider;
class CollisionManager;
class Color;

class SceneDemo : public BaseScene {
public:
	SceneDemo();
	~SceneDemo();

public:
	void load();
	void initialize();
	void finalize();

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

	std::shared_ptr<SphereCollider> parentCollider;
	std::shared_ptr<SphereCollider> childCollider;
	std::shared_ptr<SphereCollider> singleCollider;
	std::shared_ptr<SphereCollider> single2Collider;

	std::unique_ptr<CollisionManager> collisionManager;
	std::unique_ptr<Camera3D> camera3D;
};
