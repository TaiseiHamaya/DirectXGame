#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <Library/Utility/Tools/ConstructorMacro.h>

class IEditorWindow {
public:
	IEditorWindow() = default;
	virtual ~IEditorWindow() = default;

	__CLASS_NON_COPYABLE(IEditorWindow)

public:
	virtual void draw() = 0;

	void draw_menu(string_literal name);

protected:
	bool isActive{ false };
	bool isHoverMain{ false };
};

#endif // DEBUG_FEATURES_ENABLE
