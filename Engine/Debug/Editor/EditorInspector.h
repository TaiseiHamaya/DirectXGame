#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IEditorWindow.h"

#include <Library/Utility/Template/Reference.h>

class EditorHierarchy;

class EditorInspector : public IEditorWindow {
public:
	void initialize();
	void start(Reference<EditorHierarchy> hierarchyRoot_);

	void draw() override;

private:
	Reference<EditorHierarchy> hierarchyRoot;
};

#endif // DEBUG_FEATURES_ENABLE
