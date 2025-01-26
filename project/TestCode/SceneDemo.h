#pragma once

#include "Engine/Runtime/Scene/BaseScene.h"

#include <memory>

class MeshInstance;
class AnimatedMeshInstance;
class SpriteInstance;
class Camera3D;
class BaseCollider;
class SphereCollider;
class AABBCollider;
class CollisionManager;
class Color4;
class RenderPath;
#include "Engine/Module/World/Light/DirectionalLight/DirectionalLightInstance.h"

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

#ifdef _DEBUG
	void debug_update() override;
#endif // _DEBUG

private:
	std::unique_ptr<RenderPath> renderPath;

	std::unique_ptr<AnimatedMeshInstance> animatedMeshInstance;
	std::unique_ptr<DirectionalLightInstance> directionalLight;

	std::unique_ptr<Camera3D> camera3D;
};
