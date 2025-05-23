#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <Library/Utility/Template/SingletonInterface.h>

#include "EditorSceneView.h"
#include "EditorHierarchy.h"
#include "EditorInspector.h"

class EditorMain final : public SingletonInterface<EditorMain> {
	__CLASS_SINGLETON_INTERFACE(EditorMain)

public:
	static void Initialize();
	static void Start();

	static void DrawBase();
	static void Draw();

	static void Finalize();

	static bool IsHoverEditorWindow();

private:
	void set_imgui_command();

private:
	bool isActiveEditor{};

	EditorSceneView sceneView;
	EditorHierarchy hierarchy;
	EditorInspector inspector;
};

#endif // DEBUG_FEATURES_ENABLE
