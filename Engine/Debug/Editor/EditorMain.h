#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <Library/Utility/Template/SingletonInterface.h>

#include <Engine/Runtime/Input/InputHandler.h>

#include "./Core/EditorGizmo.h"
#include "./Core/EditorSceneList.h"
#include "./Core/EditorSelectObject.h"
#include "./Window/EditorHierarchy.h"
#include "./Window/EditorInspector.h"
#include "./Window/EditorSceneView.h"
#include "RemoteObject/EditorDeletedObjectPool.h"

class EditorMain final : public SingletonInterface<EditorMain> {
	__CLASS_SINGLETON_INTERFACE(EditorMain)

public:
	static void Initialize();
	static void Finalize();
	static void Setup();

	static void DrawBase();
	static void Draw();

	static bool IsHoverEditorWindow();

private:
	void set_imgui_command();

private:
	bool isActiveEditor{};

	std::optional<std::string> switchSceneName;

	InputHandler<KeyID> input;

	EditorSceneView sceneView;
	EditorHierarchy hierarchy;
	EditorInspector inspector;
	EditorGizmo gizmo;
	EditorSelectObject selectObject;
	EditorDeletedObjectPool deletedPool;
	EditorSceneList sceneList;
};

#endif // DEBUG_FEATURES_ENABLE
