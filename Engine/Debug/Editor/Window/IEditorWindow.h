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

	bool is_active() const;

protected:
	bool isActive{ false };
};

#endif // DEBUG_FEATURES_ENABLE
