#pragma once

#include "Engine/Module/World/Camera/Camera3D.h"

#include "Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h"
#include "Engine/Module/World/Mesh/StaticMeshInstance.h"
#include "Engine/Runtime/Input/InputHandler.h"

class EditorSceneView;

class EditorDebugCamera final : public Camera3D {
public:


public:
	void initialize() override;
	void update();
	void update_affine() override;

private:
	std::unique_ptr<StaticMeshInstance> constraint;
	r32 offset{ -5.0f };
	InputHandler<MouseID> mouseInputHandler;

private:
	static inline Reference<EditorSceneView> sceneView{ nullptr };

public:
	static void Setup(Reference<EditorSceneView> sceneView_);
};
