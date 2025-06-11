#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

class IEditorCommand {
public:
	IEditorCommand() = default;
	virtual ~IEditorCommand() = default;

	__CLASS_NON_COPYABLE(IEditorCommand)

public:
	virtual void execute() = 0;
	virtual void undo() = 0;
};
