#pragma once

#include "Engine/Runtime/Scene/BaseScene.h"

#include <memory>

#include "Engine/Resources/Audio/AudioPlayer.h"

class MeshInstance;
class SpriteInstance;
class Camera3D;
class BaseCollider;
class SphereCollider;
class CollisionManager;
class Color4;
class RenderPath;
#include "Engine/Module/World/Light/DirectionalLight/DirectionalLightInstance.h"

#include "Engine/Module/Render/RenderNode/Object3DNode/Object3DNode.h"
#include "Engine/Module/Render/RenderNode/Sprite/SpriteNode.h"
#include "Engine/Module/Render/RenderNode/Particle/ParticleBillboardNode/ParticleBillboardNode.h"
#include "Engine/Module/Render/RenderNode/Particle/ParticleMeshNode/ParticleMeshNode.h"

#include "Engine/Module/Render/RenderNode/Outline/OutlineNode.h"
#include "Engine/Module/World/Particle/ParticleEmitterInstance.h"

#include "Engine/Resources/Json/JsonResource.h"
#include "Engine/Debug/ImGui/ImGuiJsonEditor/ImGuiValueEditor.h"

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
	std::unique_ptr<DirectionalLightInstance> directionalLight;

	std::unique_ptr<ParticleEmitterInstance> particleEmitter;

	std::unique_ptr<Camera3D> camera3D;
};
