#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <Library/Utility/Template/SingletonInterface.h>

#include <Engine/Runtime/Input/InputHandler.h>

#include "EditorGizmo.h"
#include "EditorHierarchy.h"
#include "EditorInspector.h"
#include "EditorSceneView.h"
#include "EditorSelectObject.h"
#include "RemoteObject/EditorDeletedObjectPool.h"

class EditorMain final : public SingletonInterface<EditorMain> {
	__CLASS_SINGLETON_INTERFACE(EditorMain)

public:
	static void Initialize();
	static void Setup();

	static void DrawBase();
	static void Draw();

	static void Finalize();

	static bool IsHoverEditorWindow();

	static void SetCamera(Reference<Camera3D> camera);

private:
	void set_imgui_command();

private:
	bool isActiveEditor{};

	InputHandler<KeyID> input;

	EditorSceneView sceneView;
	EditorHierarchy hierarchy;
	EditorInspector inspector;
	EditorGizmo gizmo;
	EditorSelectObject selectObject;
	EditorDeletedObjectPool deletedPool;
};

#endif // DEBUG_FEATURES_ENABLE
