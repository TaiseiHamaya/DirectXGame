#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IEditorWindow.h"

#include <Library/Utility/Template/Reference.h>

class EditorSelectObject;

class EditorInspector : public IEditorWindow {
public:
	void initialize();
	void setup(Reference<EditorSelectObject> selectObject_);

	void draw() override;

private:
	Reference<EditorSelectObject> selectObject;
};

#endif // DEBUG_FEATURES_ENABLE
