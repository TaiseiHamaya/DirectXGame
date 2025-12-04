#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <Library/Utility/Template/SingletonInterface.h>

#include "./Core/EditorGizmo.h"
#include "./Core/EditorSceneList.h"
#include "./Core/EditorSelectObject.h"
#include "./Window/EditorHierarchy.h"
#include "./Window/EditorInspector.h"
#include "./Window/EditorRenderDAG.h"
#include "./Window/EditorSceneView.h"
#include "./Window/EditorScreenResult.h"
#include "Engine/Runtime/Input/InputHandler.h"
#include "RemoteObject/EditorDeletedObjectPool.h"

class EditorMain final : public SingletonInterface<EditorMain> {
	__CLASS_SINGLETON_INTERFACE(EditorMain)

public:
	static void Initialize();
	static void Finalize();
	static void Setup();

	static void DrawBase();
	static void Draw();

public:
	static void SetActiveEditor(bool isActive);

	static bool IsHoverEditorWindow();

	static bool IsEndApplicationForce();

	static void SeveScene();

	static void SetHotReload();

private:
	void set_imgui_command();

private:
	bool isActiveEditor{ true };

	bool isClosedEditor{ false };
	bool isEndApplicationForce{ false };

	bool isHotReload{ false };

	std::optional<std::string> switchSceneName;

	InputHandler<KeyID> input;

	EditorSceneView sceneView;
	EditorScreenResult screenResult;
	EditorHierarchy hierarchy;
	EditorInspector inspector;
	EditorGizmo gizmo;
	EditorSelectObject selectObject;
	EditorDeletedObjectPool deletedPool;
	EditorSceneList sceneList;
	EditorRenderDAG renderDAG;
};

#endif // DEBUG_FEATURES_ENABLE
