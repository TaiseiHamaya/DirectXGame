#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <Library/Utility/Tools/ConstructorMacro.h>

namespace szg {

class IEditorCommand {
public:
	IEditorCommand() = default;
	virtual ~IEditorCommand() = default;

	SZG_CLASS_MOVE_ONLY(IEditorCommand)

public:
	virtual void execute() = 0;
	virtual void undo() = 0;
};

}; // szg

#endif // DEBUG_FEATURES_ENABLE
